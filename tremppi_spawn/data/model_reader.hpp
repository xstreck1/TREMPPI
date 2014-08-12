#pragma once

#include <tremppi_common/network/model.hpp>

#include <json/json.h>

namespace ModelReader {
	// Konvert model in JSON to Model object
	Model jsonToModel(const Json::Value & elements) {
		Model model;

		for (const Json::Value node : elements["nodes"]) {
			Model::ModelComp specie;

			string id = node["data"]["id"].asString();
			specie.id = lexical_cast<CompID, string>(id);
			specie.name = node["data"]["Name"].asString();
			specie.max_activity = node["data"]["MaxActivity"].asInt();

			for (const Json::Value edge : elements["edges"]) {
				if (edge["data"]["target"].asString() == id) {
					Model::Regulation regulation;

					string source = edge["data"]["source"].asString();
					regulation.source = lexical_cast<CompID, string>(source);
					regulation.threshold = edge["data"]["Threshold"].asInt();
					regulation.label = edge["data"]["Label"].asString();

					specie.regulations.emplace_back(move(regulation));
				}
			}

			model.components.emplace_back(move(specie));
		}

		return model;
	}

	// Read the JSON file
	Json::Value readFile(ifstream & file) {
		Json::Value root;

		Json::Reader reader;
		bool parsingSuccessful = reader.parse(file, root);
		if (!parsingSuccessful)
			throw runtime_error("Failed to parse configuration. " + reader.getFormattedErrorMessages());

		return root;
	}
}