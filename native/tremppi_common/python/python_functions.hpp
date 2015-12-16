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

class PythonFunctions 
{
	bpy::object main_module;
	bpy::object main_namespace;

	PythonFunctions();
	PythonFunctions(const PythonFunctions &) = delete;
	PythonFunctions& operator=(const PythonFunctions&) = delete;
	PythonFunctions(PythonFunctions &&) = delete;
	PythonFunctions& operator=(PythonFunctions&&) = delete;
public:
	static PythonFunctions & getInstance();

	void exec(const string & command);

	static string reformPath(const bfs::path & path);

	template <typename ResultType>
    void eval(const string & command, ResultType & result);

	static void configure(string widget);

	static void throw_python_exception();
};

template<typename ResultType>

void PythonFunctions::eval(const string & command, ResultType & result) 
{
	try {
		bpy::object value = bpy::eval(command.c_str(), main_namespace);
		result = bpy::extract<ResultType>(value);
	}

	catch (bpy::error_already_set const &)
	{
		throw_python_exception();
	}
}
