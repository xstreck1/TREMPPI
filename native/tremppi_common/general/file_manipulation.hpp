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

#include <json/json.h>

#include "../header.h"
#include "common_functions.hpp"


namespace FileManipulation 
{
	//
	void copyDir(bfs::path const & source, bfs::path const & destination);

	//
	Json::Value parseJSON(const bfs::path & input_path);

	//
	void writeJSON(const bfs::path & output_path, const Json::Value & data);

	//
	void replaceInFile(bfs::path const & file, const string & original, const string & replacement);
}
