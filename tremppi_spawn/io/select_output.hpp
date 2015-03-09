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
				val["field"] = column;
				val["editable"]["type"] = "int";
				val["editable"]["min"] = 0;
				val["editable"]["max"] = model.components[t_ID].max_activity;
				columns.append(val);
			}
		}

		return columns;
	}
}