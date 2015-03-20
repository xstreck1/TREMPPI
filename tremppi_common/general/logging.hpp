#pragma once

#include "../header.h"
#include "common_functions.hpp"

using phase_no = uint64_t;

struct LogPhase {
	phase_no step_count;
	phase_no step_no;
	string description;
	size_t depth;
};

class Logging {
	static vector<LogPhase> phases;
	static bool phase_bit;
	static bool initiated;

	static const int LINE_LENGTH = 80;
	static const int NUM_CHARS = 8;
	static const int PAD = 3;

	Logging(const Logging & o) = delete;
	Logging& operator=(const Logging & o) = delete;
public:
	Logging();
	~Logging();

	// 
    static void newPhase(const string & _desc, const phase_no _step_count = 0);

	//
	static void killPhase();

	//
	static void step();

	// 
	static void exceptionMessage(const exception & e, const int err_no);
};
