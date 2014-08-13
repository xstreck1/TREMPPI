#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/data_info.hpp>
#include <tremppi_common/network/definitions.hpp>
#include <json/json.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Checks the model for the correctness of the syntax, except for the constraints (those need kinetics to be built first)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
					throw runtime_error("Could not convert max for " + quote(node["data"]["id"].asString()) + " to integer. " + e.what());
				}
				if (act_level < 1 || act_level > 9)
					throw runtime_error(quote(node["data"]["id"].asString()) + " has declared maximal activity level " + node["data"]["MaxActivity"].asString() + ".Only [1,9] is allowed.");

			}
		}

		// Guarantees that the names in nodes meet the requirements for an ID and are readable
		void checkNames(const Json::Value & nodes) {
			for (const Json::Value node : nodes) {
				string name;
				try {
					name = node["data"]["id"].asString();
				}
				catch (exception & e) {
					throw runtime_error(string("Could not obtain a specie Name. Did you remember to add parenthesis? ") + e.what());
				}
				if (!DataInfo::isValidSpecName(name))
					throw runtime_error(quote(name) + " is an invalid specie Name. ID must start with a letter and only letters, numbers and underscore are allowed.");
			}
		}

		// Control one endpoint of an edge
		void checkEdgeEnd(const Json::Value & edge, const string & type, const map<string, ActLevel> & components) {
			string name;
			try {
				name = edge["data"][type].asString();
			}
			catch (exception & e) {
				throw runtime_error(string("Could not obtain a " + type + " of an edge.Did you remember to add parenthesis ? ") + e.what());
			}
			if (!hasKey(components, name)) {
				throw runtime_error("The " + type + " " + name + " does not exists");
			}
		}

		// Control if the edges have their ids in the component list
		void checkEdges(const Json::Value & edges, const map<string, ActLevel> & components) {
			for (const Json::Value edge : edges) {
				checkEdgeEnd(edge, string("source"), components);
				checkEdgeEnd(edge, string("target"), components);
			}
		}

		const inline string getEdgeName(const Json::Value & edge) {
			return(quote("(" + edge["data"]["source"].asString() + "," + edge["data"]["target"].asString() + ")"));
		}

		// Control if the thresholds are in the range of their source
		void checkThresholds(const Json::Value & edges, const map<string, ActLevel> & components) {
			for (const Json::Value edge : edges) {
				int threshold;
				string source = edge["data"]["source"].asString();
				try {
					threshold = edge["data"]["Threshold"].asInt();
				}
				catch (exception & e) {
					throw runtime_error("Could not convert treshold for " + getEdgeName(edge) + " to integer." + e.what());
				}
				if (threshold < 1 || threshold > components.at(source))
					throw runtime_error(getEdgeName(edge) + " has a Threshold of " + to_string(threshold) +
					". Only[1" + ", " + to_string(components.at(source)) + "] is allowed.");
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
		checkMaxes(elements["nodes"]);
		map<string, ActLevel> components = DataInfo::getComponents(elements["nodes"]);
		checkEdges(elements["edges"], components);
		checkThresholds(elements["edges"], components);
		checkLabels(elements["edges"]);
	}
}