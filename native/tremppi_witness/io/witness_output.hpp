#pragma once

#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>

namespace WitnessOutput {
	inline string remStep(const string & state) {
		return state.substr(0, state.find("-"));
	}

	inline string getStep(const string & state) {
		return state.substr(state.find("-") + 1);
	}

	Json::Value convert(const set<pair<string, string>> & transitions) {
		Json::Value elements;

		// Add transitions
		Json::Value & edges = elements["edges"];
		set<string> states;
		for (const pair<string, string> & transition : transitions) {
			Json::Value edge;
			edge["data"]["id"] = to_string(elements["edges"].size());
			edge["data"]["source"] = remStep(transition.first);
			edge["data"]["target"] = remStep(transition.second);
			edge["data"]["Step"] = getStep(transition.first);
			edges.append(edge);
			states.insert(edge["data"]["source"].asString());
			states.insert(edge["data"]["target"].asString());
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