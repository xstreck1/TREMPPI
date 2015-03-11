#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/data_info.hpp>
#include <tremppi_common/network/definitions.hpp>
#include <json/json.h>

#include "../data/model_translators.hpp"
#include "../data/kinetics_translators.hpp"

namespace SelectOutput {
	Json::Value getNewColumns(const Model & model, const Kinetics & kinetics) {
		Json::Value columns;

		for (CompID t_ID : crange(model.components.size())) {
			for (auto & param : kinetics.components[t_ID].params) {
				string column = KineticsTranslators::makeConcise(param, model.components[t_ID].name);

				Json::Value val;
				val["field"] = column + "_val";
				val["editable"]["type"] = "int";
				val["editable"]["min"] = 0;
				val["editable"]["max"] = model.components[t_ID].max_activity;
				val["editable"]["step"] = 1;
				columns.append(val);
			}
		}

		return columns;
	}

	Json::Value getRegulations(const Model & model) {
		Json::Value regulations;

		for (const Model::ModelComp & comp : model.components) {
			Json::Value regulated;

			regulated["id"] = comp.name;
			for (const Model::Regulation & source : comp.regulations) {
				regulated["regulators"].append(model.components[source.s_ID].name);
			}

			regulations.append(regulated);
		}

		return regulations;
	}
}