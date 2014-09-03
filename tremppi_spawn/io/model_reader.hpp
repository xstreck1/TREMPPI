#pragma once

#include <json/json.h>

#include "../data/model_translators.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Functions that read a JSON file into a model.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace ModelReader {
	// Konvert model in JSON to Model object
	Model jsonToModel(const Json::Value & elements) {
		Model model;

		CompID ID = 0;
		string last_name = "";
		for (const Json::Value node : elements["nodes"]) {
			Model::ModelComp specie;

			specie.ID = ID++;

			specie.name = node["data"]["id"].asString();
			if (last_name > specie.name)
				throw runtime_error("Components must be ordered lexicographically, " + quote(last_name) + ">" + quote(specie.name));
			last_name = specie.name;

			specie.max_activity = node["data"]["MaxActivity"].asInt();

			for (const Json::Value constraint : node["data"]["Constraints"]) {
				specie.constraints.emplace_back(constraint.asString());
			}

			model.components.emplace_back(move(specie));
		}

		for (const Json::Value edge : elements["edges"]) {
			const CompID source_id = ModelTranslators::findID(model, edge["data"]["source"].asString());
			const CompID target_id = ModelTranslators::findID(model, edge["data"]["target"].asString());

			Model::Regulation regulation{
				source_id,
				static_cast<ActLevel>(edge["data"]["Threshold"].asInt()),
				edge["data"]["Label"].asString()
			};

			model.components[target_id].regulations.emplace_back(move(regulation));
		}

		return model;
	}
}