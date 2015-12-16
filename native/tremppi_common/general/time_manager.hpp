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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Class that allows to multiple clock for run-time measurement.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace TimeManager 
{
	using Clock = pair<string, chrono::steady_clock::time_point>; ///< Clock - contain a start time referenced by a name of the clock.
	extern map<string, chrono::steady_clock::time_point> clocks; ///< Vector of clocks, private for each cpp file
	extern chrono::system_clock::time_point start_time;

	/*
	* sets the starting point
	*/
	void setStartTime();

		/**
	 * Starts a clock with given name and, if it is requsted by user, outputs the info.
	 *
	 * @param clock_name	unique ID of the clock that will also be send on the output
	 * @param silen   If true, then the start is not printed.
	 */
	void startClock(const string clock_name);

	/**
	 * Outputs current runtime of the clock as a double floating point precision amount of seconds.
	 *
	 * @param clock_name	name of the clock to output (also appears on the output)
	 */
	void writeClock(const string clock_name);

	/**
	 * Erases the given clock from the clock-list.
	 *
	 * @brief killClock
	 * @param clock_name
	 */
	void killClock(const string clock_name);

	/**
	 * Starts the clock anew.
	 *
	 * @brief resetClock
	 * @param clock_name
	 */
	void resetClock(const string clock_name);

	/**
	 * @return	the current time as a string
	 */
	 string getTime();

	 string getTimeStamp();

	/**
	* Starts the given function, measures and outputs its time.
	*
	* @brief measureFunction
	* @param f
	* @param name
	*/
	template<class Function>

	void measureFunction(Function f, const string & name) 
	{
		startClock(name);
		f();
		writeClock(name);
		killClock(name);
	}
};