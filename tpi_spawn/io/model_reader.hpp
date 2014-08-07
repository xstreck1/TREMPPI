#pragma once
#include <tpi_common/common_functions.hpp>
#include <json/json.h>

namespace ModelReader {
	// Read the JSON file
	Json::Value readFile(ifstream & file) {
		Json::Value root;

		Json::Reader reader;
		bool parsingSuccessful = reader.parse(file, root);
		if (!parsingSuccessful)
			throw runtime_error("Failed to parse configuration. " + reader.getFormattedErrorMessages());

		return root;
	}

	namespace {
		// Guarantees that the maximal values are integers and that they are convertible to int
		void checkMaxes(const Json::Value & nodes) {
			for (const Json::Value node : nodes) {
				int act_level;
				try {
					act_level = node["data"]["MaxActivity"].asInt();
				}
				catch (exception & e) {
					throw runtime_error("Could not convert max for " + node["data"]["id"].asString() + " to integer." + e.what());
				}
				if (act_level < 1 || act_level > 9)
					throw runtime_error(node["data"]["id"].asString() + " has maximal activity level " + node["data"]["MaxActivity"].asString() + ".Only [1,9] is allowed.");

			}
		}

		// Guarantees that the names in nodes meet the requirements for an ID and are readable
		void checkNames(const Json::Value & nodes) {
			for (const Json::Value node : nodes) {
				string name;
				try {
					name = node["data"]["name"].asString();
				}
				catch (exception & e) {
					throw runtime_error(string("Could not obtain a specie ID. Did you remember to add parenthesis? ") + e.what());
				}
				if (!DataInfo::isValidSpecName(name))
					throw runtime_error(name + " is an invalid specie ID. ID must start with a letter and only letters, numbers and underscore are allowed.");
			}
		}

		// Obtain components and their max levels
		map<string, ActLevel> getComponents(const Json::Value & nodes) {
			map<string, ActLevel> components;

			for (const Json::Value node : nodes) 
				components.insert(make_pair(node["data"]["name"].asString(), node["data"]["MaxActivity"].asInt()));

			return components;
		}

		// Control if the edges are of correct types and the values semantically meet the requirements
		void checkEdges(const Json::Value & edges, const map<string, ActLevel> & components) {
			for (const Json::Value edge : edges) {
				string source, target;
				int threshold; 
				try {
					target = edge["data"]["target"].asString();
				}
				catch (exception & e) {
					throw runtime_error(string("Could not obtain a source of an edge. Did you remember to add parenthesis? ") + e.what());
				}
				try {
					source = edge["data"]["source"].asString();
				}
				catch (exception & e) {
					throw runtime_error("Could not obtain a source of an edge with target " + quote(target) + ".Did you remember to add parenthesis ? " + e.what());
				}
				try {
					threshold = edge["data"]["Threshold"].asInt();
				}
				catch (exception & e) {
					throw runtime_error("Could not convert treshold for (" + quote(target) + "," + quote(source) + ") to integer." + e.what());
				}
				if (!hasKey(components, target))
					throw runtime_error("Target " + quote(target) + " is not a component.");
				if (!hasKey(components, source))
					throw runtime_error("Source " + quote(source) + " is not a component.");
				if (threshold < 1 || threshold > components.find(source)->second)
					throw runtime_error("(" + quote(target) + ", " + quote(source) + " has a Threshold of " + to_string(threshold) + 
					".Only[1" + ", " + to_string(components.find(source)->second) + "] is allowed.");

			}
		}
	}

	// Control that the values are not missing and are of the right form
	void controlSemantics(const Json::Value & elements) {
		checkNames(elements["nodes"]);
		checkMaxes(elements["nodes"]);
		map<string, ActLevel> components = getComponents(elements["nodes"]);
		checkEdges(elements["edges"], components);
	}
}