#pragma once

#include "../header.h"
#include "common_functions.hpp"

class Logging {
	size_t step_count;
	size_t substep_count;
	size_t step_no;
	size_t substep_no;
	string phase_desc;
	string subphase_desc;
	const size_t MAX_WIDHT = 35; // Maximal lenght of the message

	Logging(const Logging & o) = delete;
	Logging& operator=(const Logging & o) = delete;
	void uniStep(bool is_sub);

public:
	Logging() = default;
	//
	void init();

	// 
	void Logging::newPhase(const string & _desc, const size_t _step_count = 0);

	// 
	void Logging::newSubPhase(const string & _desc, const size_t _substep_count = 0);

	//
	void step();

	//
	void subStep();

	// 
	void exceptionMessage(const exception & e, const int err_no);
};

extern Logging logging;