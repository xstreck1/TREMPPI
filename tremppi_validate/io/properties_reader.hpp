#pragma once

#include <json/json.h>
#include <tremppi_common/network/data_info.hpp>

#include "../data/property_info.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Functions that read an (almost) JSON file into automata.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace PropertiesReader {
	pair<ActLevel, ActLevel> readBoundary(const string & value, const string & property_name, const string & comp_name);
	//
	PathCons getTransitionConstraint(const string & constraint, const string & comp_name);
	// Konvert model in (almost) JSON to Model object
	vector<PropertyInfo> jsonToProperties(Json::Value & properties);
}