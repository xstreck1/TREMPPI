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

#include "../data/data.hpp"

namespace Output 
{
	// Add a function column for each component 
	void addColumns(const RegFuncs & reg_funcs, sqlite3pp::database & db);

	// 
	string plitToFormula(const string & component, const Levels & trhs, const PLit & plit, const ActLevel max);

	//
	string pminToFormula(const RegFuncs & funcs, const RegFunc & reg_func, const PMin & pmin, const ActLevel target_val);

	//
	string pdnfToFormula(const RegFuncs & funcs, const RegFunc & reg_func, const vector<PDNF> & pdnfs);
}