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
	size_t step_count;
	size_t substep_count;
	size_t step_no;
	size_t substep_no;
	string phase_desc;
	string subphase_desc;

	vector<LogPhase> phases;
	bool phase_bit;

	static const int LINE_LENGTH = 80;
	static const int NUM_CHARS = 8;
	static const int PAD = 3;

	Logging(const Logging & o) = delete;
	Logging& operator=(const Logging & o) = delete;
public:
	Logging() = default;
	//
	void init();

	// 
	void Logging::newPhase(const string & _desc, const size_t _step_count = 0);

	//
	void Logging::killPhase();

	//
	void step();

	// 
	void exceptionMessage(const exception & e, const int err_no);
};

extern Logging logging;