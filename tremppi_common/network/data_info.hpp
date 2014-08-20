#pragma once

#include "../header.h"
#include "definitions.hpp"
#include <json/json.h>

namespace DataInfo {
	// Controls if the names is formed as [a-zA-Z][a-zA-Z0-9_]*
	bool isValidSpecName(const string & spec_name);

	// Obtain components and their max levels
	map<string, ActLevel> getComponents(const Json::Value & nodes);
}