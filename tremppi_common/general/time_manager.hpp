#pragma once

#include "../header.h"
#include "common_functions.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Class that allows to multiple clock for run-time measurement.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace TimeManager {
	using Clock = pair<string, chrono::steady_clock::time_point>; ///< Clock - contain a start time referenced by a name of the clock.
	static map<string, chrono::steady_clock::time_point> clocks; ///< Vector of clocks, private for each cpp file

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

	/**
	* Starts the given function, measures and outputs its time.
	*
	* @brief measureFunction
	* @param f
	* @param name
	*/
	template<class Function>
	void measureFunction(Function f, const string & name) {
		startClock(name);
		f();
		writeClock(name);
		killClock(name);
	}
};