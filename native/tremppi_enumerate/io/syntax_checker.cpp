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

#include "syntax_checker.hpp"

void SyntaxChecker::checkMaxes(const Json::Value & nodes)
{
	for (const Json::Value node : nodes) {
		int act_level;

		try
		{
			act_level = node["data"]["MaxActivity"].asInt();
		}

		catch (exception & e)
		{
			throw runtime_error("Could not convert max for " + quote(node["data"]["Name"].asString()) + " to integer. " + e.what());
		}
		if (act_level < 1 || act_level > 9)
		{
			throw runtime_error(quote(node["data"]["Name"].asString()) + " has declared maximal activity level " + node["data"]["MaxActivity"].asString() + ".Only [1,9] is allowed.");
		}
	}
}

void SyntaxChecker::checkIDs(const Json::Value & nodes)
{
	for (const Json::Value & node : nodes)
	{
		string name;

		try
		{
			name = node["data"]["id"].asString();
		}

		catch (exception & e)
		{
			throw runtime_error(string("Could not obtain a specie ID. Did you remember to add parenthesis? ") + e.what());
		}
		if (!DataInfo::isValidName(name))
			throw runtime_error(quote(name) + " is an invalid specie ID. ID must start with a letter and only letters, numbers and underscore are allowed.");
	}
}

void SyntaxChecker::checkNames(const Json::Value & nodes)
{
	for (const Json::Value & node : nodes)
	{
		string name;

		try
		{
			name = node["data"]["Name"].asString();
		}

		catch (exception & e)
		{
			throw runtime_error(string("Could not obtain a specie Name. Did you remember to add parenthesis? ") + e.what());
		}
		if (!DataInfo::isValidName(name))
			throw runtime_error(quote(name) + " is an invalid specie Name. Name must start with a letter and only letters, numbers and underscore are allowed.");
	}
}

map<string, string> SyntaxChecker::idsToNames(const Json::Value & nodes)
{
	map<string, string> result;

	for (const Json::Value & node : nodes)
		result.insert({ node["data"]["id"].asString(), node["data"]["Name"].asString() });

	return result;
}

void SyntaxChecker::checkEdgeEnd(const Json::Value & edge, const string & type, const map<string, ActLevel> & components)
{
	string name;

	try
	{
		name = edge["data"][type].asString();
	}

	catch (exception & e)
	{
		throw runtime_error(string("Could not obtain a " + type + " of an edge.Did you remember to add parenthesis ? ") + e.what());
	}

	if (!hasKey(components, name))
	{
		throw runtime_error("The " + type + " " + name + " does not exists");
	}
}

void SyntaxChecker::checkEdges(const Json::Value & edges, const map<string, ActLevel> & components)
{
	for (const Json::Value & edge : edges)
	{
		checkEdgeEnd(edge, string("source"), components);
		checkEdgeEnd(edge, string("target"), components);
	}
}


const string SyntaxChecker::getEdgeName(const Json::Value & edge, const map<string, string> & ids_to_names)
{
	return(quote("(" + ids_to_names.at(edge["data"]["source"].asString()) + "," + ids_to_names.at(edge["data"]["target"].asString()) + ")"));
}

void SyntaxChecker::checkThresholds(const Json::Value & edges, const map<string, ActLevel> & components, const map<string, string> & ids_to_names)
{
	for (const Json::Value & edge : edges)
	{
		int threshold;
		string source = edge["data"]["source"].asString();

		try
		{
			threshold = edge["data"]["Threshold"].asInt();
		}

		catch (exception & e)
		{
			throw runtime_error("Could not convert the threshold for " + getEdgeName(edge, ids_to_names) + " to integer." + e.what());
		}
		if (threshold < 1 || threshold > components.at(source))
			throw runtime_error(getEdgeName(edge, ids_to_names) + " has a Threshold of " + to_string(threshold) +
				". Only[1" + ", " + to_string(components.at(source)) + "] is allowed.");
	}
}

void SyntaxChecker::checkLabels(const Json::Value & edges, const map<string, string> & ids_to_names)
{
	for (const Json::Value & edge : edges)
	{
		string label;

		try
		{
			label = edge["data"]["Label"].asString();
		}

		catch (exception & e)
		{
			throw runtime_error("Could not obtain a label of an edge " + getEdgeName(edge, ids_to_names) + ". Did you remember to add parenthesis ? " + e.what());
		}

		if (label_list.count(label) == 0)
		{
			throw runtime_error("The label " + label + " is not known to the system.");
		}
	}
}

void SyntaxChecker::controlSemantics(const Json::Value & elements)
{
	checkIDs(elements["nodes"]);
	checkNames(elements["nodes"]);
	auto ids_to_names = idsToNames(elements["nodes"]);
	checkMaxes(elements["nodes"]);
	map<string, ActLevel> components = DataInfo::getComponents(elements["nodes"]);
	checkEdges(elements["edges"], components);
	checkThresholds(elements["edges"], components, ids_to_names);
	checkLabels(elements["edges"], ids_to_names);
}