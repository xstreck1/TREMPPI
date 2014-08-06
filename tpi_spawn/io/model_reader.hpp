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
					act_level = node["data"]["max"].asInt();
				}
				catch (exception & e) {
					throw runtime_error("Could not convert max for " + node["data"]["id"].asString() + " to integer." + e.what());
				}
				if (act_level < 1 || act_level > 9)
					throw runtime_error(node["data"]["id"].asString() + " has maximal activity level " + node["data"]["max"].asString() + ".Only [1,9] is allowed.");

			}
		}

		// Guarantees that the names in nodes meet the requirements for an ID and are readable
		void checkNames(const Json::Value & nodes) {
			for (const Json::Value node : nodes) {
				string name;
				try {
					name = node["data"]["max"].asString();
				}
				catch (exception & e) {
					throw runtime_error(string("Could not obtain a specie ID. Did you remember to add parenthesis? ") + e.what());
				}
				if (!DataInfo::isValidSpecName(name))
					throw runtime_error(name + " is an invalid specie ID. ID must start with a letter and only letters, numbers and underscore are allowed.");
			}
		}
	}

	// Control that the values are not missing and are of the right form
	void controlSemantics(const Json::Value & elements) {
		checkNames(elements["nodes"]);
		checkMaxes(elements["nodes"]);
	}
}