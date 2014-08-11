#pragma once

#include "../network/definitions.hpp"
#include "../general/data_conv.hpp"
#include "sqlite3pp_func.hpp"

// Regulatory graph info, stored by a component
struct RegInfo {
	string name;
	ActLevel max;
	map<string, size_t> columns;
	pair<size_t, size_t> columns_range;
	map<string, Levels> regulators;
};

/// Reads parametrizations form given file
namespace DatabaseReader {
	const string NAMES_COLUMN = "Name";
	const string MAX_LEVEL_COLUMN = "MaxActivity";
	const string REGULATOR_COLUMN = "Regulator";
	const string TARGET_COLUMN = "Target";
	const string THRESHOLDS_COLUMN = "Thresholds";

	// Obtain maximal levels for the individual components
	ActLevel getMaxLevel(const string & name, sqlite3pp::database & db) {
		int result = 0;

		sqlite3pp::query qry(db, ("SELECT " + NAMES_COLUMN + ", " + MAX_LEVEL_COLUMN + " FROM " + COMPONENTS_TABLE).c_str());
		for (auto row : qry) {
			boost::tuple<string, int> data = row.get_columns<string, int>(0, 1);
			if (data.get<0>() == name)
				result = max(result, data.get<1>());
		}

		return static_cast<ActLevel>(result);
	}

	// Get regulators of the given component
	map<string, Levels> obtainRegulators(const string & component, sqlite3pp::database & db) {
		map<string, Levels>  result;

		sqlite3pp::query qry(db, ("SELECT " + REGULATOR_COLUMN + ", " + TARGET_COLUMN + ", " + THRESHOLDS_COLUMN + " FROM " + REGULATIONS_TABLE).c_str());
		for (auto row : qry) {
			string source, target, thresholds;
			row.getter() >> source >> target >> thresholds;
			if (target == component) {
				// Convert the string to values
				vector<string> treshs_str;
				split(treshs_str, thresholds, is_any_of(","));
				Levels treshs_act(treshs_str.size());
				transform(WHOLE(treshs_str), begin(treshs_act), lexical_cast<ActLevel, string>);
				// Assign
				result[source] = treshs_act;
			}
		}

		return result;
	}

	//
	Levels getThrFromContexts(const vector<string> & columns_name, const size_t regul_i) {
		Levels result;

		for (const size_t context_i : cscope(columns_name)) {
			string digits = getAllMatches("_(\\d*)\\b", columns_name[context_i], 1).front();
			result.emplace_back(lexical_cast<ActLevel>(digits[regul_i]));
		}

		return result;
	}

	//
	Levels getThrsFromContext(const string & column_name) {
		Levels result;

		string digits = getAllMatches("_(\\d*)\\b", column_name, 1).front();
		result.resize(digits.size());
		transform(WHOLE(digits), begin(result), lexical_cast<ActLevel, char>);

		return result;
	}

	// Read headers for all the regulatory functions
	RegInfo readRegInfo(const string & name, sqlite3pp::database & db) {
		ActLevel max = getMaxLevel(name, db);
		auto columns = func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("K_" + name + ".*"), db);
		pair<size_t, size_t> columns_range = DataConv::indices2range(columns);
		auto regulators = obtainRegulators(name, db);

		return RegInfo{ name, max, move(columns), move(columns_range), move(regulators) };
	}

	query selectionFilter(const map<string, size_t> & columns, const string & selection, sqlite3pp::database & db) {
		string columns_list = alg::join(DataConv::columns2list(columns), ", ");
		string where_clause = selection.empty() ? "" : " WHERE " + selection;
		return query(db, ("SELECT " + columns_list + " FROM " + PARAMETRIZATIONS_TABLE + where_clause).c_str());
	}
};
