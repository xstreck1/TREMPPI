#pragma once

#include <tremppi_common/header.h>

#include "../data/data_storage.hpp"

namespace Output {
	Json::Value functionalData(const FunsData & funs_data) {
		Json::Value elements;

		// Write the nodes
		string nodes;
		for (const FunData & fun : funs_data) {
			Json::Value node, data;
			data["id"] = fun.info.name;
			data["name"] = fun.info.name;
			data["Bias"] = fun.mean;
			data["Std"] = fun.std_dev;
			data["Max"] = fun.info.max_activity;

			node["data"] = data;
			elements["nodes"].append(node);
		}

		// Write the edges
		for (const FunData & fun : funs_data) {
			size_t reg_i = 0;
			for (const int i : cscope(fun.corrs)) {
				if (fun.corrs[i] == 0)
					continue;
				Json::Value edge, data;

				data["source"] = fun.info.name;
				data["target"] = funs_data[i].info.name;
				data["Pearson"] = fun.corrs[i]; 
				
				edge["data"] = data;
				elements["edges"].append(edge);
			}
		}
		return elements;
	}
}