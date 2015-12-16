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
#include "../network/definitions.hpp"
#include "common_functions.hpp"

namespace DataConv 
{
	//
	pair<size_t, size_t> indices2range(const map<size_t, string> & columns);

	//
	vector<string> columns2list(const map<size_t, string> & columns);

	//
	Levels getThrFromContexts(const vector<string> & columns_name, const size_t regul_i);

	//
	Levels getThrsFromContext(const string & column_name);

	// create a map that provides indeces for thresholds
	map<ActLevel, size_t> getThresholdToIndex(const Levels & thresholds);
}


