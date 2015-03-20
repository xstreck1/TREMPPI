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
		for (const Json::Value & property : root) {
			PropertyAutomaton automaton;

			// Skip those that are not in use
			if (!property["desc"][0]["values"]["Verify"].asBool())
				continue;

			automaton.name = property["desc"][0]["values"]["Name"].asString();
			automaton.prop_type = property["desc"][0]["values"]["Type"].asString();
			automaton.experiment = property["desc"][0]["values"]["Experiment"].asString();

			StateID ID = 0;

			// Add an accepting first state
			if (automaton.prop_type == "Cycle") {
				automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, true, {}, PropertyAutomaton::Edges({ { 1, "tt" } }) });
				ID++;
			}

			for (const Json::Value & measurement : property["data"]) {
				string constraint = measurement["values"]["Measurement"].asString();

				string negation = "!(" + constraint + ")";

				PropertyAutomaton::Edges edges = { { ID, negation }, { ID + 1, constraint } };

				vector<string> stables_list;
				string stables = measurement["values"]["Stables"].asString();
				if (!stables.empty())
					boost::split(stables_list, stables, boost::is_any_of(","));

				automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, false, move(stables_list), move(edges) });

				// Stables for the next state (once the values requried here are reached)
				ID++;

			}
			// Loop to the first
			if (automaton.prop_type == "Cycle") {
				automaton.states.back().edges.back().target_ID = 0;
			}
			// Add a new state to the end that has a loop and compies the requirement for stables
			if (automaton.prop_type == "TimeSeries") {
				automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, true, {}, PropertyAutomaton::Edges({ { ID, "tt" } }) });
			}
			
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
		for (const Json::Value & property : root) {
			// Check name
			DataInfo::isValidName(property["desc"][0]["Name"].asString());
			// Check prop type
			const string PROP_TYPE = property["desc"][0]["values"]["Type"].asString();
			if (find(WHOLE(PropType), PROP_TYPE) == end(PropType))
				throw runtime_error("Unknown property type: " + PROP_TYPE);
			// Check property specific
			if (PROP_TYPE == "TimeSeries")
				checkTimeSeries(property);
		}		
	}
}