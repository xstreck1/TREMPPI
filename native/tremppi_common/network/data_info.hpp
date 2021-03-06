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

#pragma once

#include "../header.h"
#include "definitions.hpp"
#include "../general/common_functions.hpp"
#include <json/json.h>

// Regulatory graph info, stored by a component
struct RegInfo 
{
	CompID ID;
	string name;
	ActLevel max_activity;
	map<size_t, string> columns; // (column index, starting from 0, based on SELECT *, column name)
	map<size_t, Levels> contexts; // (column index, starting from 0, based on SELECT *, column thresholds)
	map<CompID, Levels> regulators; // (Regulator ID, all thresholds)
	map<size_t, Configurations > requirements; // To each column assign the values of the reugulators that fall into the context
};
using RegInfos = vector<RegInfo>;

namespace DataInfo 
{
	// Controls if the names is formed as [a-zA-Z][a-zA-Z0-9_]*
	bool isValidName(const string & spec_name);

	// Obtain components and their max levels
	map<string, ActLevel> getComponents(const Json::Value & nodes);

	// @return	names of all the components
	vector<string> getAllNames(const RegInfos & reg_infos);

	//
	ActLevel getMaxLevel(const RegInfos & reg_infos);

	//
	vector<string> getAllContexts(const RegInfos & reg_infos);

	//
	CompID getCompID(const RegInfos & reg_infos, const string & name);

	//
	CompID getCompID(const map<string, CompID> & components, const string & name);

	// for a regulator from the given regulatory info, obtain its ordinal index
	size_t RegIDToRegNo(const RegInfo & reg_info, const CompID ID);

	// for a regulator from the given index number, obtain its ID
	CompID RegNoToRegID(const RegInfo & reg_info, const size_t reg_no);

	// 
	size_t columnNoToColumnID(const RegInfo & reg_info, const size_t column_no);

	//
	size_t columnIDToColumnNo(const RegInfo & reg_info, const size_t column_ID);

	// Which context do I obtain if I remove from the context in the column "column_no" of the component "reg_info" the regulation by "reg_ID" with the threshold "threshold" 
	size_t getColumnWithout(const RegInfo & reg_info, const size_t column_no, const CompID reg_ID, const ActLevel threshold);

	//
	vector<string> columnsVector(const RegInfo & reg_info);

	//
	vector<Levels> contextsVector(const RegInfo & reg_info);
}

