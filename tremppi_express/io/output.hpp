#pragma once

#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/database/database_reader.hpp>

#include "../data/data.hpp"

namespace Output {
	// Add a function column for each component 
	void addColumns(const RegFuncs & reg_funcs, sqlite3pp::database & db) {
		for (const RegFunc & reg_func : reg_funcs) {
			sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, "F_" + reg_func.info.name, "STRING", db);
		}
	}

	// 
	string plitToFormula(const string & component, const Levels & trhs, const PLit & plit, const ActLevel max) {
		string result;
		// Skip completely if the literal contains all the thresholds
		if (plit.size() == trhs.size())

		// If multivalued, use Post literal
		if (max > 1) {
			// Build the thresholds for this regultion
			Levels thresholds = trhs;
			thresholds.insert(begin(thresholds), 0);
			thresholds.push_back(max + 1);
			auto trhs_it = begin(thresholds);

			// Build the literal
			result += component + "{";
			for (int pval : plit) {
				while (*trhs_it < pval)
					trhs_it++;
				while (pval < *(trhs_it + 1)) {
					result += to_string(pval++);
				}
			}
			result += "}";
			
		}
		// Otherwise use boolean literal
		else {
			if (plit[0] == 0)
				result = "!";
			result += component;
		}

		return result;
	}

	//
	string pminToFormula(const RegFuncs & funcs, const RegFunc & reg_func, const PMin & pmin, const ActLevel target_val) {
		string result;

		// Add front value in case the component is not boolean or has no regulators (constant)
		if (reg_func.info.max_activity > 1 || reg_func.info.regulators.empty())
			result += to_string(target_val) + "&";

		// Add values of all the regulators
		size_t reg_i = 0;
		for (auto regulation : reg_func.info.regulators) {
			string lit_form = plitToFormula(funcs[regulation.first].info.name, regulation.second, pmin[reg_i], funcs[regulation.first].info.max_activity);
			if (!lit_form.empty())
				result += lit_form + "&";
			reg_i++;
		}

		if (!result.empty() && result.back() == '&')
			result.resize(result.size() - 1);
		return result;
	}

	//
	string pdnfToFormula(const RegFuncs & funcs, const RegFunc & reg_func, const vector<PDNF> & pdnfs) {
		string result;

		for (const ActLevel target_val : cscope(pdnfs)) {
			for (const PMin & pmin : pdnfs[target_val]) {
				string min_form = pminToFormula(funcs, reg_func, pmin, target_val);

				result += min_form + "|";
			}
		}

		if (result.empty() || result== "|")
			result = "0";
		else if (result.back() == '|')
			result.resize(result.size() - 1);
		return result;
	}
}