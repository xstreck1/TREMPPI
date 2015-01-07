#pragma once

#include "../header.h"
#include "common_functions.hpp"

struct LogPhase {
	size_t step_count;
	size_t step_no;
	string description;
	size_t depth;
};

class Logging {
	vector<LogPhase> phases;
	bool phase_bit;
	bool initiated;

	static const int LINE_LENGTH = 80;
	static const int NUM_CHARS = 8;
	static const int PAD = 3;

	Logging(const Logging & o) = delete;
	Logging& operator=(const Logging & o) = delete;
public:
	Logging();
	//
	void init(const bfs::path & work_path, const string & program_name);

	// 
    void newPhase(const string & _desc, const size_t _step_count = 0);

	//
    void killPhase();

	//
	void step();

	// 
	void exceptionMessage(const exception & e, const int err_no);
};

extern Logging logging;
