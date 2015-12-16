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

#include "../network/definitions.hpp"
#include "../network/data_info.hpp"
#include "../general/data_conv.hpp"
#include "sqlite3pp_func.hpp"

/// Reads parametrizations form given file
class DatabaseReader 
{
	const string NAMES_COLUMN = "Name";
	const string MAX_LEVEL_COLUMN = "MaxActivity";
	const string REGULATOR_COLUMN = "Source";
	const string TARGET_COLUMN = "Target";
	const string THRESHOLD_COLUMN = "Threshold";
	map<string, CompID> components_dict;
	vector<string> components;
	map<string, ActLevel> maxes_dict;
	vector<ActLevel> maxes;

	// Obtain components and their ids
	void readComponents(sqlite3pp::database & db);

	// Obtain maximal levels 
    void readMaxes(sqlite3pp::database & db);

	//
	vector<Levels> obtainRequirements(const string & context, const map<CompID, Levels> & regulators, sqlite3pp::database & db);

	// Get regulators of the given component
	map<CompID, Levels> readRegulators(const string & name, sqlite3pp::database & db);

public:
	// Read headers for all the regulatory functions
	RegInfos readRegInfos(sqlite3pp::database & db);

	// 
	static sqlite3pp::query selectionFilter(const map<size_t, string> & columns, const string & selection, sqlite3pp::database & db);

	//
	static sqlite3pp::query selectionIDs(const string & selection, sqlite3pp::database & db);

	// Obtain the lines of the sqlite3 selection terms for each selected line in select
	static string getSelectionTerm();

	//
	static vector<string> getSelectionList();

	// Obtain the the names of the current selection lines 
	static vector<string> getSelectionNames();

	//
	static int getSelectionSize(const string & selection, sqlite3pp::database & db);
};
