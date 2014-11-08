#pragma once

#include "../header.h"
#include "definitions.hpp"
#include "../general/common_functions.hpp"
#include <json/json.h>

// Regulatory graph info, stored by a component
struct RegInfo {
	CompID ID;
	string name;
	ActLevel max_activity;
	map<size_t, string> columns;
	map<CompID, Levels> regulators;
	map<size_t, vector<Levels> > requirements; // To each collumn assign the values of the reugulators that fall into the context
};
using RegInfos = vector<RegInfo>;

namespace DataInfo {
	// Controls if the names is formed as [a-zA-Z][a-zA-Z0-9_]*
	bool isValidName(const string & spec_name);

	// Obtain components and their max levels
	map<string, ActLevel> getComponents(const Json::Value & nodes);

	// @return	names of all the components
	vector<string> getAllNames(const RegInfos & reg_infos);

	//
	ActLevel getMaxLevel(const RegInfos & reg_infos);

	//
	vector<string> getAllContexts(const RegInfos & reg_infos);

	//
	CompID getCompID(const RegInfos & reg_infos, const string & name);

	//
	CompID getCompID(const map<string, CompID> & components, const string & name);

	//
	map<CompID, vector<vector<size_t>>> getColumnsOfThresholds(const RegInfo & reg_info);

	// for a regultor from the given regulatory info, obtain its ordinal index
	size_t inline getRegulatorI(const CompID ID, const RegInfo & reg_info);
}

