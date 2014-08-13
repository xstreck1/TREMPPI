#pragma once

#include <tremppi_common/database/sqlite3pp_func.hpp>

#include "../data/data.hpp"

namespace Output {
	// 
	void addColumns(const RegFuncs & reg_funcs, SQLAdapter & database) {
		for (const RegFunc & reg_func : reg_funcs) {
			database.prepareColumn(PARAMETRIZATIONS_TABLE, "F_" + reg_func.name, "STRING");
		}
	}

	//
	string plitToFormula(const pair<string, Levels> & reg, const PLit & plit, const ActLevel max) {
		string result;
		// Skip completely if the literal contains all the thresholds
		if (plit.size() == reg.second.size())

		// If multivalued, use Post literal
		if (max > 1) {
			// Build the thresholds for this regultion
			Levels thresholds = reg.second;
			thresholds.insert(begin(thresholds), 0);
			thresholds.push_back(max + 1);
			auto trhs_it = begin(thresholds);

			// Build the literal
			result += reg.first + "{";
			for (int pval : plit) {
				while (*trhs_it < pval)
					trhs_it++;
				while (pval < *(trhs_it + 1)) {
					result += to_string(pval++) + ",";
				}
			}
			result.back() = '}';
			
		}
		// Otherwise use boolean literal
		else {
			if (plit[0] == 0)
				result = "!";
			result += reg.first;
		}

		return result;
	}

	//
	string pminToFormula(const map<string, ActLevel> & maxes, const RegFunc & reg_func, const PMin & pmin, const ActLevel target_val) {
		string result;

		// Add front value in case the component is not boolean
		if (reg_func.max > 1)
			result += to_string(target_val) + "&";

		// Add values of all the regulators
		size_t reg_i = 0;
		for (auto regulation : reg_func.regulators) {
			string lit_form = plitToFormula(regulation, pmin[reg_i], maxes.at(regulation.first.c_str()));
			if (!lit_form.empty())
				result += lit_form + "&";
			reg_i++;
		}

		result.resize(result.size() - 1);
		return result;
	}

	string pdnfToFormula(const map<string, ActLevel> & maxes, const RegFunc & reg_func, const vector<PDNF> & pdnfs) {
		string result;

		for (const ActLevel target_val : cscope(pdnfs)) {
			for (const PMin & pmin : pdnfs[target_val]) {
				string min_form = pminToFormula(maxes, reg_func, pmin, target_val);

				// If all the terms do not cover the whole option space
				if (min_form.size() != 1) {
					result += min_form + "|";
				}
			}
		}
		result.resize(result.size() - 1);
		return result;
	}
}