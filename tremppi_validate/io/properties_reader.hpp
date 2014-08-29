#pragma once

#include <json/json.h>
#include <tremppi_common/network/data_info.hpp>

#include "../data/property_automaton.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Functions that read an (almost) JSON file into automata.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace PropertiesReader {
	// Konvert model in (almost) JSON to Model object
	vector<PropertyAutomaton> jsonToProperties(const Json::Value & root) {
		vector<PropertyAutomaton> automata;
		for (const Json::Value & property : root["properties"]) {
			PropertyAutomaton automaton;

			StateID ID = 0;
			for (const Json::Value & measurement : property["measurements"]) {
				string constraint = measurement.asString();
				string negation = "!(" + constraint + ")";
				PropertyAutomaton::Edges edges = { { ID, negation }, { ID + 1, constraint } };
				automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, false, edges });
				ID++;
			}

			automaton.name = property["name"].asString();
			automaton.prop_type = property["type"].asString();
			automaton.experiment = property["experiment"].asString();
			automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, true, PropertyAutomaton::Edges ()});
			automata.emplace_back(move(automaton));
		}
		return automata;
	}

	namespace {
		void checkTimeSeries(const Json::Value & property) {
			// Nothing to check specifically, 
		}
	}

	void checkSemantics(const Json::Value & root) {
		for (const Json::Value & property : root["properties"]) {
			// Check name
			DataInfo::isValidName(property["name"].asString());
			// Check prop type
			const string PROP_TYPE = property["type"].asString();
			if (find(WHOLE(PropType), PROP_TYPE) == end(PropType))
				throw runtime_error("Unknown property type: " + PROP_TYPE);
			// Check property specific
			if (PROP_TYPE == "TimeSeries")
				checkTimeSeries(property);
		}		
	}
}