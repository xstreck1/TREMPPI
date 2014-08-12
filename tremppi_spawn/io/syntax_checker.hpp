#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/data_info.hpp>
#include <tremppi_common/network/definitions.hpp>
#include <json/json.h>

namespace SyntaxChecker {
	namespace {
		// Guarantees that the maximal values are integers and that they are convertible to int
		void checkMaxes(const Json::Value & nodes) {
			for (const Json::Value node : nodes) {
				int act_level;
				try {
					act_level = node["data"]["MaxActivity"].asInt();
				}
				catch (exception & e) {
					throw runtime_error("Could not convert max for " + quote(node["data"]["Name"].asString()) + " to integer. " + e.what());
				}
				if (act_level < 1 || act_level > 9)
					throw runtime_error(quote(node["data"]["Name"].asString()) + " has declared maximal activity level " + node["data"]["MaxActivity"].asString() + ".Only [1,9] is allowed.");

			}
		}

		// Guarantees that the names in nodes meet the requirements for an ID and are readable
		void checkNames(const Json::Value & nodes) {
			for (const Json::Value node : nodes) {
				string name;
				try {
					name = node["data"]["Name"].asString();
				}
				catch (exception & e) {
					throw runtime_error(string("Could not obtain a specie Name. Did you remember to add parenthesis? ") + e.what());
				}
				if (!DataInfo::isValidSpecName(name))
					throw runtime_error(quote(name) + " is an invalid specie Name. ID must start with a letter and only letters, numbers and underscore are allowed.");
			}
		}

		// Constrol that Ids are numeric and ordered
		void checkIDs(const Json::Value & nodes, const vector<string> & names) {
			for (const Json::Value node : nodes) {
				CompID ID;
				try {
					string ID_string = node["data"]["id"].asString();
					ID = static_cast<CompID>(boost::lexical_cast<CompID, string>(ID_string));
				}
				catch (exception & e) {
					throw runtime_error("Could not obtain and convert id for " + quote(node["data"]["Name"].asString()) + " to integer. " + e.what());
				}
				CompID computed_id = getIndex(names, node["data"]["Name"].asString());
				if (ID != computed_id)
					throw runtime_error("Component " + quote(node["data"]["Name"].asString()) + " has an id out of order. The ids must be assigned in a lexicographical order of names.");

			}
		}

		// Control one endpoint of an edge
		void checkEdgeEnd(const Json::Value & edge, const string & type, const size_t node_count) {
			string name;
			CompID ID;
			try {
				name = edge["data"][type].asString();
			}
			catch (exception & e) {
				throw runtime_error(string("Could not obtain a " + type + " of an edge.Did you remember to add parenthesis ? ") + e.what());
			}
			try {
				ID = lexical_cast<CompID, string>(name);
			}
			catch (exception & e) {
				throw runtime_error("The ID " + quote(name) + " is not convertible to int. " + e.what());
			}
			if (ID >= node_count)
				throw runtime_error("The ID " + quote(ID) + " is not a valid ID. ");
		}

		// Control if the edges have their ids in the component list
		void checkEdges(const Json::Value & edges, const size_t node_count) {
			for (const Json::Value edge : edges) {
				checkEdgeEnd(edge, string("source"), node_count);
				checkEdgeEnd(edge, string("target"), node_count);
			}
		}

		const inline string getEdgeName(const Json::Value & edge) {
			return(quote("(" + edge["data"]["source"].asString() + "," + edge["data"]["target"].asString() + ")"));
		}

		// Control if the thresholds are in the range of their source
		void checkThresholds(const Json::Value & edges, const vector<pair<string, ActLevel> > & components) {
			for (const Json::Value edge : edges) {
				int threshold;
				CompID source = lexical_cast<CompID, string>(edge["data"]["source"].asString());
				try {
					threshold = edge["data"]["Threshold"].asInt();
				}
				catch (exception & e) {
					throw runtime_error("Could not convert treshold for " + getEdgeName(edge) + " to integer." + e.what());
				}
				if (threshold < 1 || threshold > components[source].second)
					throw runtime_error(getEdgeName(edge) + " has a Threshold of " + to_string(threshold) +
					".Only[1" + ", " + to_string(components[source].second) + "] is allowed.");
			}
		}

		// Checks if all the edge labels are correct
		void checkLabels(const Json::Value & edges) {
			for (const Json::Value edge : edges) {
				string label;
				try {
					label = edge["data"]["Label"].asString();
				}
				catch (exception & e) {
					throw runtime_error("Could not obtain a label of an edge " + getEdgeName(edge) + ". Did you remember to add parenthesis ? " + e.what());
				}
				// Check if the label belongs to the list
				if (getIndex(Label::All, label) == INF)
					throw runtime_error(getEdgeName(edge) + " has the label " + quote(label) + "which is not a known label");
			}
		}
	}

	// Control that the values are not missing and are of the right form
	void controlSemantics(const Json::Value & elements) {
		checkNames(elements["nodes"]);
		vector<string> names;
		for (auto node : elements["nodes"]) 
			names.emplace_back(node["data"]["Name"].asString());
		sort(WHOLE(names));
		checkIDs(elements["nodes"], names);
		checkMaxes(elements["nodes"]);
		vector<pair<string, ActLevel>> components = DataInfo::getComponents(elements["nodes"]);
		checkEdges(elements["edges"], components.size());
		checkThresholds(elements["edges"], components);
		checkLabels(elements["edges"]);
	}
}