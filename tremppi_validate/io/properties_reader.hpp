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

			for (const Json::Value & measurement : property["data"]) {
				string constraint = measurement["values"]["Measurement"].asString();

				string negation = "!(" + constraint + ")";

				string stables = measurement["values"]["Stables"].asString();
				vector<string> stables_list;
				if (!stables.empty())
					boost::split(stables_list, stables, boost::is_any_of(","));

				PropertyAutomaton::Edges edges = { { ID, negation }, { ID + 1, constraint } };

				automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, false, move(stables_list), move(edges) });

				ID++;
			}
			// Add mirror of the first state that is not accepting.
			if (automaton.prop_type == "Cycle") {
				automaton.states[automaton.states.size() - 1].edges[1].target_ID = 0;
				automaton.states.push_back(automaton.states[0]);
				automaton.states[0].final = true;
				automaton.states[0].edges[0].target_ID = automaton.states.size() - 1;
				automaton.states[automaton.states.size() - 1].edges[0].target_ID = automaton.states.size() - 1;
			}
			// Add a new state to the end that is just accepting
			else if (automaton.prop_type == "TimeSeries") {
				automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, true, {}, PropertyAutomaton::Edges() });
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