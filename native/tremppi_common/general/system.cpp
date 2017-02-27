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

#include "system.hpp"

// Define variables
string TremppiSystem::PROGRAM_NAME;
bfs::path TremppiSystem::HOME_PATH;
bfs::path TremppiSystem::WORK_PATH;
bfs::path TremppiSystem::EXEC_PATH;
bfs::path TremppiSystem::BIN_PATH;
bfs::path TremppiSystem::DATA_PATH;
bool TremppiSystem::called_from_server;

void TremppiSystem::set(const char * name, const char * arg, const string & _WORK_PATH) 
{
	TremppiSystem::PROGRAM_NAME = name;
	TremppiSystem::WORK_PATH = _WORK_PATH;
	TremppiSystem::DATA_PATH = TremppiSystem::WORK_PATH / DATA_FOLDER;
	TremppiSystem::BIN_PATH = bfs::absolute(bfs::path{ arg });
	TremppiSystem::EXEC_PATH = bfs::current_path();
	TremppiSystem::called_from_server = false; // Will be true only if called with the --server parameter

	// Test the home
	char* home_path = getenv("TREMPPI_HOME");
	if (home_path != NULL)
	{
		bfs::path test_path = bfs::path{ home_path };
		if (bfs::is_directory(test_path / "python") && bfs::is_directory(test_path / "javascript"))
		{
			TremppiSystem::HOME_PATH = test_path;
			return;
		}
	}

	// Descend three steps, test for tremppi directory
	bfs::path test_path = BIN_PATH;
	for (auto i : crange(3))
	{
		if (bfs::is_directory(test_path / "python") && bfs::is_directory(test_path / "javascript"))
		{
			TremppiSystem::HOME_PATH = test_path;
			return;
		}
		test_path = test_path.parent_path();
	}

	throw runtime_error("Binary " + TremppiSystem::BIN_PATH.string() + "is not homed and TREMPPI_HOME not found. Either set up the system variable of the same name pointing to the TREMMPI folder, or put the binary in the TREMPPI/bin folder.");
}

void TremppiSystem::initiate(const string & name, int argc, char **argv) {
	TimeManager::setStartTime();
	try
	{
		string path = ".";
		for (const size_t arg_no : crange(argc)) 
		{
			if (string(argv[arg_no]) == "--path") 
			{
				path = argv[arg_no + 1];
			}
		}
		for (const size_t arg_no : crange(argc))
		{
			if (string(argv[arg_no]) == "--server")
			{
				TremppiSystem::called_from_server = true;
			}
		}
		TremppiSystem::set(name.c_str(), argv[0], path);
	}

	catch (exception & e)
	{
		cerr << e.what() << endl;
		exit(1);
	}
}

ArgPtr::ArgPtr(const vector<string>& values)
{
	argv = new char*[values.size() + 1];

	// Add binary name
	argv[0] = new char[TremppiSystem::BIN_PATH.string().size() + 1];
	strcpy(argv[0], TremppiSystem::BIN_PATH.string().c_str());

	// Copy new arguments
	argc = 1;
	for (const string & val : values)
	{
		argv[argc] = new char[val.size() + 1];
		strcpy(argv[argc], val.c_str());
		argc++;
	}
}

ArgPtr::~ArgPtr()
{
	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}
	delete[] argv;
}

char ** ArgPtr::getArgv() 
{ 
	return argv; 
}

int ArgPtr::getArgc() 
{ 
	return argc; 
}
