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
#include "common_functions.hpp"

using PhaseNo = uint64_t;

struct LogPhase 
{
	PhaseNo step_count;
	PhaseNo step_no;
	string description;
	size_t depth;
};

class Logging 
{
	static ofstream file;
	static vector<LogPhase> phases;
	static int last_progress_val;
	static constexpr int PRECISION = 3;
	static constexpr float SHIFT_FACTOR = 1000;

public:
	Logging();
	NO_COPY_SHORT(Logging);
	~Logging();
	//
	static ofstream & getFile();
	//
	static void acquireFile();
	//
	static void releaseFile();
	// 
	static void newPhase(const string & _desc, const PhaseNo _step_count = 0);
	//
	static void killPhase();
	//
	static void step();
	// 
	static void exceptionMessage(const exception & e, const int err_no);
};

#define DEBUG_LOG Logging::getFile() << std::endl << std::flush << TimeManager::getTime() << " I: "
#define WARNING_LOG Logging::getFile() << std::endl << std::flush << TimeManager::getTime() << " W: "
#define ERROR_LOG Logging::getFile() << std::endl << std::flush << TimeManager::getTime() << " E: "