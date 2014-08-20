#pragma once

#include "../header.h"
#include "common_functions.hpp"

class Logging {
public:
	size_t phase_no;
	size_t phase_count;
	size_t step_count;
	size_t step_no;
	string desc;
	const size_t MAX_PHASES = 99;
	const size_t MAX_WIDHT = 60; // Maximal lenght of the message

	//
	void init(const string LOGFILE = "logfile.log");

	// 
	void newPhase(const size_t _step_count, const string & _desc);

	void step();

	// 
	void exceptionMessage(const exception & e, const int err_no);
};

extern Logging logging;