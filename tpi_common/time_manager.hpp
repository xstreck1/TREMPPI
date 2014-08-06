#pragma once

#include "common_functions.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Class that allows to multiple clock for run-time measurement.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace TimeManager {
	using Clock = pair<string, chrono::steady_clock::time_point>; ///< Clock - contain a start time referenced by a name of the clock.
	static map<string, chrono::steady_clock::time_point> clocks; ///< Vector of clocks.

	/**
	 * Starts a clock with given name and, if it is requsted by user, outputs the info.
	 *
	 * @param clock_name	unique ID of the clock that will also be send on the output
	 * @param silen   If true, then the start is not printed.
	 */
	void startClock(const string clock_name) {
		if (clocks.find(clock_name) != clocks.end())
			throw runtime_error(clock_name + " clock already started. Error when trying to create.");
		clocks.insert(make_pair(clock_name, chrono::steady_clock::now()));

		BOOST_LOG_TRIVIAL(info) << "Clock " << quote(clock_name) << " started.";
	}

	/**
	 * Outputs current runtime of the clock as a double floating point precision amount of seconds.
	 *
	 * @param clock_name	name of the clock to output (also appears on the output)
	 */
	void writeClock(const string clock_name) {
		// Find the clock and output time difference
		if (clocks.find(clock_name) != clocks.end()) {
			auto start_tp = clocks.find(clock_name)->second;
			auto current_tp = chrono::steady_clock::now();
			auto time_span = chrono::duration_cast<chrono::duration<double>>(current_tp - start_tp);
			BOOST_LOG_TRIVIAL(info) << "Clock " << quote(clock_name) << " counted " << time_span.count() << "s.";
		}
		else  // If you do not find them, fail
			throw runtime_error(clock_name + " have not been started until now. Error requesting the clock.");
	}

	/**
	 * Erases the given clock from the clock-list.
	 *
	 * @brief killClock
	 * @param clock_name
	 */
	void killClock(const string clock_name) {
		if (clocks.find(clock_name) == clocks.end())
			throw runtime_error(clock_name + " clock are not present. Error when trying to kill.");
		clocks.erase(clock_name);
	}

	/**
	 * Starts the clock anew.
	 *
	 * @brief resetClock
	 * @param clock_name
	 */
	void resetClock(const string clock_name) {
		killClock(clock_name);
		startClock(clock_name);
	}

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

	string getTime() {
		string result;

		chrono::system_clock::time_point today = chrono::system_clock::now();
		time_t tt = chrono::system_clock::to_time_t(today);
		result = ctime(&tt);
		result.resize(result.size() - 1); // Remove newline

		return result;
	}
};