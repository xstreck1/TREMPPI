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

#include <boost/python/detail/wrap_python.hpp>
#include <tremppi_common/header.h>
#include <tremppi_common/general/system.hpp>

#include "run_script.hpp"

int run_script(const string command, int argc, char ** argv) 
{
	TremppiSystem::initiate("tremppi_" + command, argc, argv);
	unique_ptr<Logging> logging;
	if (command != "browse") 
	{
		logging.reset(new Logging());
	}

	// Check if the script exists
	bfs::path file_path = TremppiSystem::HOME_PATH / bfs::path{ "python" } / bfs::path{ command + ".py" };

	if (!bfs::exists(file_path)) 
	{
		cerr << file_path.string() << " is called, but does not exist.";
		return 1;
	}

	wchar_t ** wargv = new wchar_t*[argc];

	for (int i : crange(argc)) 
	{
		wargv[i] = new wchar_t[strlen(argv[i]) + 1];
		mbstowcs(wargv[i], argv[i], strlen(argv[i]) + 1);
	}

	// Copy the script path to the first argument
	string path_text = file_path.string();
	wchar_t * program_name = new wchar_t[strlen(path_text.c_str()) + 1];
	mbstowcs(program_name, path_text.c_str(), strlen(path_text.c_str()) + 1);
	swap(program_name, wargv[0]);

	// Initialize Python interpreter, the intepreter gets the name of this binary as the argv[0]
	Py_SetProgramName(program_name);

	Py_Initialize();
	PySys_SetArgv(argc, wargv);

	// Run python
	std::ifstream fin(file_path.string().c_str());
	std::string contents((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
	PyRun_SimpleString(contents.c_str());

	delete[] program_name;
	for (int i : crange(argc))
		delete[] wargv[i];
	delete[] wargv;
	return 0;
}

int tremppi_init(int argc, char ** argv) 
{
	return run_script("init", argc, argv);
}

int tremppi_browse(int argc, char ** argv) 
{
	return run_script("browse", argc, argv);
}

int tremppi_configure(int argc, char ** argv) 
{
	return run_script("configure", argc, argv);
}

int tremppi_update(int argc, char ** argv) 
{
	return run_script("update", argc, argv);
}

int tremppi_clean(int argc, char ** argv)
{
	return run_script("clean", argc, argv);
}

int tremppi_finalize(int argc, char ** argv)
{
	return run_script("finalize", argc, argv);
}

int tremppi_erase(int argc, char ** argv)
{
	return run_script("erase", argc, argv);
}

int tremppi_fix(int argc, char ** argv)
{
	return run_script("fix", argc, argv);
}

int tremppi_release(int argc, char ** argv)
{
	return run_script("release", argc, argv);
}