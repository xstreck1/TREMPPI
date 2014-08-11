#pragma once

#include <tremppi_common/network/model.hpp>

#include <json/json.h>

namespace ModelReader {
	// Konvert model in JSON to Model object
	Model jsonToModel(const Json::Value & elements) {
		Model model;

		for (const Json::Value node : elements["nodes"]) {
			Model::ModelSpecie specie;

			specie.name = node["data"]["Name"].asString();
			specie.max_value = node["data"]["MaxActivity"].asInt();

			for (const Json::Value edge : elements["edges"]) {
				if (edge["data"]["target"].asString() == specie.name) {
					Model::Regulation regulation;

					regulation.source = edge["data"]["source"].asString();
					regulation.threshold = edge["data"]["treshold"].asInt();

					specie.regulations.emplace_back(move(regulation));
				}
			}

			model.species.emplace_back(move(specie));
		}

		return model;
	}
}