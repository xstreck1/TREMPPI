#include "time_manager.hpp"

void TimeManager::startClock(const string clock_name) {
	if (clocks.find(clock_name) != clocks.end())
		throw runtime_error(clock_name + " clock already started. Error when trying to create.");
	clocks.insert(make_pair(clock_name, chrono::steady_clock::now()));

	BOOST_LOG_TRIVIAL(info) << "Clock " << quote(clock_name) << " started.";
}

void TimeManager::writeClock(const string clock_name) {
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

void TimeManager::killClock(const string clock_name) {
	if (clocks.find(clock_name) == clocks.end())
		throw runtime_error(clock_name + " clock are not present. Error when trying to kill.");
	clocks.erase(clock_name);
}

void TimeManager::resetClock(const string clock_name) {
	killClock(clock_name);
	startClock(clock_name);
}

string TimeManager::getTime() {
	string result;

	chrono::system_clock::time_point today = chrono::system_clock::now();
	time_t tt = chrono::system_clock::to_time_t(today);
	result = ctime(&tt);
	result.resize(result.size() - 1); // Remove newline

	return result;
}