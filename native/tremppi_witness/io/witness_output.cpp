#pragma once

#include "witness_output.hpp"

string WitnessOutput::remStep(const string & state)
{
	return state.substr(0, state.find("-"));
}

string WitnessOutput::getStep(const string & state)
{
	return state.substr(state.find("-") + 1);
}

Json::Value WitnessOutput::convert(const map<pair<string, string>, size_t> & transitions, ParamNo max_count)
{
	Json::Value elements;

	// Add transitions
	Json::Value & edges = elements["edges"];
	edges.resize(0); // Make sure it's not null
	set<string> states;

	for (const pair<pair<string, string>, size_t> & transition : transitions)
	{
		Json::Value edge;
		edge["data"]["id"] = to_string(elements["edges"].size());
		edge["data"]["source"] = remStep(transition.first.first);
		edge["data"]["target"] = remStep(transition.first.second);
		edge["data"]["Step"] = getStep(transition.first.first);
		edge["data"]["Frequency"] = transition.second / static_cast<double>(max_count);
		edges.append(edge);
		states.insert(edge["data"]["source"].asString());
		states.insert(edge["data"]["target"].asString());
	}

	// Add nodes
	Json::Value & nodes = elements["nodes"];
	nodes.resize(0);
	for (const string & state : states)
	{
		Json::Value node;
		node["data"]["id"] = state;
		node["data"]["Name"] = state.substr(0, state.size() - 1);
		nodes.append(node);
	}

	return elements;
}