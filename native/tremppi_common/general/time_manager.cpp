#include "time_manager.hpp"
#include "logging.hpp"

map<string, chrono::steady_clock::time_point> TimeManager::clocks; ///< Vector of clocks, private for each cpp file
chrono::system_clock::time_point TimeManager::start_time;

void TimeManager::setStartTime() 
{
	TimeManager::start_time = chrono::system_clock::now();
}

void TimeManager::startClock(const string clock_name) 
{
	if (clocks.find(clock_name) != clocks.end())
		throw runtime_error(clock_name + " clock already started. Error when trying to create.");
	clocks.insert(make_pair(clock_name, chrono::steady_clock::now()));

	DEBUG_LOG << "Clock " << quote(clock_name) << " started.";
}

void TimeManager::writeClock(const string clock_name) 
{
	// Find the clock and output time difference
	if (clocks.find(clock_name) != clocks.end()) 
	{
		auto start_tp = clocks.find(clock_name)->second;
		auto current_tp = chrono::steady_clock::now();
		auto time_span = chrono::duration_cast<chrono::duration<double>>(current_tp - start_tp);
		DEBUG_LOG << "Clock " << quote(clock_name) << " counted " << time_span.count() << "s.";
	}
	else  // If you do not find them, fail
		throw runtime_error(clock_name + " have not been started until now. Error requesting the clock.");
}

void TimeManager::killClock(const string clock_name) 
{
	if (clocks.find(clock_name) == clocks.end())
		throw runtime_error(clock_name + " clock are not present. Error when trying to kill.");
	clocks.erase(clock_name);
}

void TimeManager::resetClock(const string clock_name) 
{
	killClock(clock_name);
	startClock(clock_name);
}

string TimeManager::getTime() 
{
	string result;

	time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
	result = ctime(&tt);
	result.resize(result.size() - 1); // Remove newline

	return result;
}

string TimeManager::getTimeStamp() 
{
	string result;

	auto makeTwoDigit = [](int val) 
	{
		string result = to_string(val);
		if (result.size() > 2)
			return result.substr(2, 2);
		else if (result.size() < 2)
			return "0" + result;
		else
			return result;
	};

	time_t tt = chrono::system_clock::to_time_t(start_time);
	tm* time = gmtime(&tt);
	result = makeTwoDigit(time->tm_year + 1900) + makeTwoDigit(time->tm_mon + 1) + makeTwoDigit(time->tm_mday)
		+ makeTwoDigit(time->tm_hour) + makeTwoDigit(time->tm_min) + makeTwoDigit(time->tm_sec);

	return result;
}