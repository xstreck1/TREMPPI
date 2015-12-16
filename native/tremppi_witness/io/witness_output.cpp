/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

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