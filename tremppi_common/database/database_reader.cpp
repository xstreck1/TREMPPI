#include "database_reader.hpp"

ActLevel DatabaseReader::getMaxLevel(const string & name, sqlite3pp::database & db) {
	int result = 0;

	sqlite3pp::query qry(db, ("SELECT " + NAMES_COLUMN + ", " + MAX_LEVEL_COLUMN + " FROM " + COMPONENTS_TABLE).c_str());
	for (auto row : qry) {
		boost::tuple<string, int> data = row.get_columns<string, int>(0, 1);
		if (data.get<0>() == name)
			result = max(result, data.get<1>());
	}

	return static_cast<ActLevel>(result);
}

map<string, Levels> DatabaseReader::obtainRegulators(const string & component, sqlite3pp::database & db) {
	map<string, Levels>  result;

	sqlite3pp::query qry(db, ("SELECT * FROM " + REGULATIONS_TABLE).c_str());
	for (auto row : qry) {
		string source, target;
		ActLevel threshold;
		row.getter() >> target >> source >> threshold;
		if (target == component) {
			result[source].emplace_back(threshold);
		}
	}

	return result;
}

Levels DatabaseReader::getThrFromContexts(const vector<string> & columns_name, const size_t regul_i) {
	Levels result;

	for (const size_t context_i : cscope(columns_name)) {
		string digits = getAllMatches("_(\\d*)\\b", columns_name[context_i], 1).front();
		result.emplace_back(lexical_cast<ActLevel>(digits[regul_i]));
	}

	return result;
}

Levels DatabaseReader::getThrsFromContext(const string & column_name) {
	Levels result;

	string digits = getAllMatches("_(\\d*)\\b", column_name, 1).front();
	result.resize(digits.size());
	transform(WHOLE(digits), begin(result), lexical_cast<ActLevel, char>);

	return result;
}

RegInfo DatabaseReader::readRegInfo(const CompID ID, const string & name, sqlite3pp::database & db) {
	ActLevel max = getMaxLevel(name, db);
	auto columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("K_" + name + "_.*"), db);
	size_t prev_index = 0;
	for (const auto & column : columns) {
		if (column.first < prev_index) {
			throw runtime_error("Database format critical error. The columns for parameters are not in lexicographical order.");
		}
		prev_index = column.first;
	}
	auto regulators = obtainRegulators(name, db);

	return RegInfo{ ID, name, max, move(columns), move(regulators) };
}

sqlite3pp::query DatabaseReader::selectionFilter(const map<size_t, string> & columns, const string & selection, sqlite3pp::database & db) {
	string columns_list = alg::join(DataConv::columns2list(columns), ", ");
	string where_clause = selection.empty() ? "" : " WHERE " + selection;
	return sqlite3pp::query(db, ("SELECT " + columns_list + " FROM " + PARAMETRIZATIONS_TABLE + where_clause).c_str());
}

sqlite3pp::query DatabaseReader::selectionIDs(const string & selection, sqlite3pp::database & db) {
	string where_clause = selection.empty() ? "" : " WHERE " + selection;
	return sqlite3pp::query(db, ("SELECT ROWID FROM " + PARAMETRIZATIONS_TABLE + where_clause).c_str());
}