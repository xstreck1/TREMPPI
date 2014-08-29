#pragma once

#include "../header.h"
#include "definitions.hpp"
#include <json/json.h>

// Regulatory graph info, stored by a component
struct RegInfo {
	CompID ID;
	string name;
	ActLevel max_activity;
	map<string, size_t> columns;
	pair<size_t, size_t> columns_range;
	map<string, Levels> regulators;
};
using RegInfos = vector<RegInfo>;

namespace DataInfo {
	// Controls if the names is formed as [a-zA-Z][a-zA-Z0-9_]*
	bool isValidSpecName(const string & spec_name);

	// Obtain components and their max levels
	map<string, ActLevel> getComponents(const Json::Value & nodes);

	/**
	* @return	names of all the components
	*/
	vector<string> getAllNames(const RegInfos & reg_infos);

	//
	ActLevel getMaxLevel(const RegInfos & reg_infos);
}

