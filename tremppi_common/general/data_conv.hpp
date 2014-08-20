#pragma once

#include "../header.h"
#include "common_functions.hpp"

namespace DataConv {
	//
	pair<size_t, size_t> indices2range(const map<string, size_t> & columns);

	//
	vector<string> columns2list(const map<string, size_t> & columns);
}


