#pragma once

#include <json/json.h>
#include <tremppi_common/network/data_info.hpp>

#include "../data/property_automaton.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Functions that read an (almost) JSON file into automata.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace PropertiesReader {
	//
	pair<ActLevel, ActLevel> readExperiment(const string & value, const string & property_name, const string & comp_name, const ActLevel max_activity);
	//
	string getStateConstraint(const string & value, const string & comp_name);
	//
	pair<string, PathCons> getTransitionConstraint(const string & constraint, const string & comp_name);
	// Konvert model in (almost) JSON to Model object
	vector<PropertyAutomaton> jsonToProperties(const RegInfos & reg_infos, Json::Value & properties);
}