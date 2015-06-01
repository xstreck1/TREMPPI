#pragma once

#include "../header.h"
#include "../network/definitions.hpp"
#include "common_functions.hpp"

namespace DataConv {
	//
	pair<size_t, size_t> indices2range(const map<size_t, string> & columns);

	//
	vector<string> columns2list(const map<size_t, string> & columns);

	//
	Levels getThrFromContexts(const vector<string> & columns_name, const size_t regul_i);

	//
	Levels getThrsFromContext(const string & column_name);

	// create a map that provides indeces for thresholds
	map<ActLevel, size_t> getThresholdToIndex(const Levels & thresholds);
}


