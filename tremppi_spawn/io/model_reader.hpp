#pragma once

#include <tremppi_common/network/model_translators.hpp>

#include <json/json.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Functions that read a JSON file into a model.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace ModelReader {
	// Konvert model in JSON to Model object
	Model jsonToModel(const Json::Value & elements) {
		Model model;

		CompID id = 0;
		string last_name = "";
		for (const Json::Value node : elements["nodes"]) {
			Model::ModelComp specie;

			specie.id = id++;

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
				model.components[source_id],
				edge["data"]["Threshold"].asInt(),
				edge["data"]["Label"].asString()
			};

			model.components[target_id].regulations.emplace_back(move(regulation));
		}

		return model;
	}

	// Read the JSON file
	Json::Value readFile(const bfs::path & input_path) {
		Json::Value root;

		// Open file
		ifstream file(input_path.string(), ios::in);
		if (!file)
			throw runtime_error("Could not open the file " + input_path.string() + " for reading.");

		// Remove JS additions
		std::stringstream buffer;
		char ch = static_cast<char>(0);
		while (ch != '{') {
			file.get(ch);
		}
		do {
			buffer.put(ch);
			file.get(ch);
		} while (ch != ';' && file.good());

		file.close();

		// Parse JSON
		Json::Reader reader;
		bool parsingSuccessful = reader.parse(buffer, root);
		if (!parsingSuccessful)
			throw runtime_error("Failed to parse configuration. " + reader.getFormattedErrorMessages());

		return root;
	}
}