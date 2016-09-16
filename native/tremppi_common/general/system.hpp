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

#include "logging.hpp"
#include "time_manager.hpp"

// The global setup for Tremppi apps - mainly sets paths to the executables and data

namespace TremppiSystem 
{
	//
	void set(const char * name, const char * arg, const string & _WORK_PATH);

	//
	void initiate(const string & name, int argc, char **argv);

	extern string PROGRAM_NAME; // Name of the program running
	extern bfs::path HOME_PATH; // The path to the home of tremppi
	extern bfs::path WORK_PATH; // The directory in which tremppi works currently
	extern bfs::path EXEC_PATH; // The path from where the program was executed
	extern bfs::path BIN_PATH; // The path to the executed binary
	extern bfs::path DATA_PATH; // The path to the data failes
	extern bool called_from_server; // True if called with the --server parameter
}

class ArgPtr
{
	int argc;
	char ** argv;

public:
	ArgPtr(const vector<string> & values);

	~ArgPtr();

	char ** getArgv();

	int getArgc();
};
