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

#include "report.hpp"
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>

Json::Value Report::createSetup(const string & selection, const string & name) 
{
	Json::Value out;

	out["setup"]["date"] = TimeManager::getTime();
	sqlite3pp::database db((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str());

	out["setup"]["select"] = selection;
	out["setup"]["s_name"] = name;
	// Get pool sizes
	int size = DatabaseReader::getSelectionSize(selection, db);
	if (size == 0) 
	{
		throw runtime_error("No models in the selection.");
	}

	out["setup"]["size"] = size;

	return out;
}
string Report::reformName(const RegInfos & reg_infos, const string & name)
{
	switch (name[0]) {
		case 'B':
			return regex_replace(name, regex("B_(.*)"), "Bias($1)");
		case 'C':
			return regex_replace(name, regex("C_(.*)"), "Cost($1)");
		case 'E':
			return regex_replace(name, regex("E_(.*)"), "Indigree($1)");
		case 'F':
			return regex_replace(name, regex("F_(.*)"), "F<sub>$1</sub>");
		case 'I':
			return regex_replace(name, regex("I_(.*)_(\\d)_(.*)"), "Impact($1,$2,$3)");
		case 'K':
			return contextToValues(reg_infos, name);
		case 'S':
			return regex_replace(name, regex("S_(.*)_(\\d)_(.*)"), "Sign($1,$2,$3)");
		case 'R':
			return regex_replace(name, regex("R_(.*)"), "Robustness($1)");
	}
}

string Report::contextToValues(const RegInfos & reg_infos, const string & context) {
	string result = "K<sub>";
	smatch sm;
	regex_match(context, sm, regex("K_(.*)_(\\d*)"));
	const string component = sm[1];
	result += component + "</sub>(";
	const string context_vals = sm[2];
	if (context_vals.empty()) 
	{
		return result + ")";
	}
	const CompID ID = DataInfo::getCompID(reg_infos, component);
	const RegInfo & reg_info = reg_infos[ID];
	for (const size_t reg_no : cscope(context_vals)) 
	{
		const CompID reg_ID = DataInfo::RegNoToRegID(reg_info, reg_no);
		Levels thresholds = reg_info.regulators.at(reg_ID);
		thresholds.insert(begin(thresholds), static_cast<ActLevel>(0));
		thresholds.push_back(reg_info.max_activity + 1);
		const ActLevel current_trehold = static_cast<int>(context_vals[reg_no]) - static_cast<int>('0');
		const ActLevel next_threshold = *(std::find(WHOLE(thresholds), current_trehold) + 1);
		result += reg_infos[reg_ID].name + "{";
		for (const ActLevel level : crange(current_trehold, next_threshold)) 
		{
			result += string(1, static_cast<char>(level + '0')) + ",";
		}
		result.back() = '}';
		result += ',';
	}
	result.back() = ')';
	return result;
}