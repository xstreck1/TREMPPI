#pragma once

#include <json/json.h>

#include "../data/property_automaton.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Functions that read an (almost) JSON file into automata.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace PropertiesReader {
	// Konvert model in (almost) JSON to Model object
	vector<PropertyAutomaton> jsonToProperties(const Json::Value & properties) {
		vector<PropertyAutomaton> automata;

		return automata;
	}

	void checkSemantics(const Json::Value & properties) {

	}
}