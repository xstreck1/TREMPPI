#pragma once

#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>

namespace WitnessOutput {
	Json::Value convert(const set<pair<string, string>> & transitions) {
		Json::Value elements;

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