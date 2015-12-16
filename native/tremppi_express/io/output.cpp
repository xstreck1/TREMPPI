/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/database/database_reader.hpp>

#include "output.hpp"

void Output::addColumns(const RegFuncs & reg_funcs, sqlite3pp::database & db)
{
	for (const RegFunc & reg_func : reg_funcs) 
	{
		sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, "F_" + reg_func.info.name, "STRING", db);
	}
}

string Output::plitToFormula(const string & component, const Levels & trhs, const PLit & plit, const ActLevel max)
{
	string result;
	// Skip completely if the literal contains all the thresholds
	if (plit.size() == trhs.size() + 1) 
	{
		return "";
	}

	// If multivalued, use Post literal
	if (max > 1)
	{
		// Build the thresholds for this regultion
		Levels thresholds = trhs;
		thresholds.insert(begin(thresholds), 0);
		thresholds.push_back(max + 1);
		auto trhs_it = begin(thresholds);

		// Build the literal
		result += component + "{";
		for (int pval : plit)
		{
			while (*trhs_it < pval)
			{
				trhs_it++;			}
			while (pval < *(trhs_it + 1))
			{
				result += to_string(pval++);
			}
		}
		result += "}";
	}

	// Otherwise use boolean literal
	else
	{
		if (plit[0] == 0) 
		{
			result = "!";
		}
		result += component;
	}

	return result;
}

string Output::pminToFormula(const RegFuncs & funcs, const RegFunc & reg_func, const PMin & pmin, const ActLevel target_val)
{
	string result;

	// Add front value in case the component is not boolean or has no regulators (constant)
	if (reg_func.info.max_activity > 1 || reg_func.info.regulators.empty())
	{
		result += to_string(target_val) + "&";
	}

	// Add values of all the regulators
	size_t reg_i = 0;
	for (auto regulation : reg_func.info.regulators)
	{
		string lit_form = plitToFormula(funcs[regulation.first].info.name, regulation.second, pmin[reg_i], funcs[regulation.first].info.max_activity);
		if (!lit_form.empty()) 
		{
			result += lit_form + "&";
		}
		reg_i++;
	}

	if (!result.empty() && result.back() == '&') 
	{
		result.resize(result.size() - 1);
	}
	return result;
}

string Output::pdnfToFormula(const RegFuncs & funcs, const RegFunc & reg_func, const vector<PDNF> & pdnfs)
{
	string result;

	for (const size_t target_val : cscope(pdnfs))
	{
		for (const PMin & pmin : pdnfs[target_val]) 
		{
			string min_form = pminToFormula(funcs, reg_func, pmin, target_val);
			result += min_form + "|";
		}
	}

	if (result.empty()) 
	{
		result = "0";
	}
	else if (result == "|") 
	{
		result = "1";
	}
	else if (result.back() == '|')
	{
		result.resize(result.size() - 1);
	}
	return result;
}