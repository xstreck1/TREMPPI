#pragma once

#include "../header.h"
#include "definitions.hpp"
#include <json/json.h>

namespace DataInfo {
	// Controls if the names is formed as [a-zA-Z][a-zA-Z0-9_]*
	bool isValidSpecName(const string & spec_name) {
		if (spec_name.empty())
			return false;
		bool valid = isalpha(spec_name[0]) || spec_name[0] == '_';
		for (char ch : spec_name)
			valid &= isalnum(ch) || ch == '_';
		return valid;
	}

	// Obtain components and their max levels
	map<string, ActLevel> getComponents(const Json::Value & nodes) {
		map<string, ActLevel>components;

		for (const Json::Value node : nodes)
			components.insert(make_pair(node["data"]["Name"].asString(), node["data"]["MaxActivity"].asInt()));

		return components;
	}
}