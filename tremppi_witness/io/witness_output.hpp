#pragma once

#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>

namespace WitnessOutput {
	Json::Value addPropert(const string & name, const string & select, sqlite3pp::database & db) {
		Json::Value elements;

		WitnessReader wit_reader;
		wit_reader.select(name, select, db);
		set<pair<string, string>> transitions;

		// Read transitions
		while (wit_reader.next()) {
			set<pair<string, string>> new_transitions = wit_reader.getWitness();
			transitions.insert(WHOLE(new_transitions));
		}

		// Add transitions
		Json::Value & edges = elements["edges"];
		set<string> states;
		for (const pair<string, string> & transition : transitions) {
			Json::Value edge;
			edge["data"]["id"] = to_string(elements["edges"].size());
			edge["data"]["source"] = transition.first;
			edge["data"]["target"] = transition.second;
			edges.append(edge);
			states.insert(transition.first);
			states.insert(transition.second);
		}

		// Add nodes
		Json::Value & nodes = elements["nodes"];
		for (const string & state : states) {
			Json::Value node;
			node["data"]["id"] = state;
			nodes.append(node);
		}

		return elements;
	}
}