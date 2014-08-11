#pragma once

#include <tremppi_common/network/model.hpp>

#include <json/json.h>

namespace ModelReader {
	// Konvert model in JSON to Model object
	Model jsonToModel(const Json::Value & elements) {
		Model model;

		for (const Json::Value node : elements["nodes"]) {
			Model::ModelSpecie specie;

			string id = node["data"]["id"].asString();
			specie.id = lexical_cast<CompID, string>(id);
			specie.name = node["data"]["Name"].asString();
			specie.max_activity = node["data"]["MaxActivity"].asInt();

			for (const Json::Value edge : elements["edges"]) {
				if (edge["data"]["target"].asString() == id) {
					Model::Regulation regulation;

					string source = edge["data"]["source"].asString();
					regulation.source = lexical_cast<CompID, string>(source);
					regulation.threshold = edge["data"]["treshold"].asInt();

					specie.regulations.emplace_back(move(regulation));
				}
			}

			model.species.emplace_back(move(specie));
		}

		return model;
	}
}