#pragma once

#include <tremppi_common/header.h>

#include "../data/data_storage.hpp"

namespace Output {
	Json::Value functionalData(const pair<string, FunsData> & funs) {
		Json::Value graph;

		// Write the nodes
		string nodes;
		for (const FunData & fun : funs.second) {
			Json::Value node, data;
			data["id"] = fun.info.name;
			data["name"] = fun.info.name;
			data["Bias"] = fun.mean;
			data["Std"] = fun.std_dev;

			node["data"] = data;
			graph["elements"]["nodes"].append(node);
		}

		// Write the edges
		for (const FunData & fun : funs.second) {
			size_t reg_i = 0;
			for (const int i : cscope(fun.corrs)) {
				if (fun.corrs[i] == 0)
					continue;
				Json::Value edge, data;

				data["source"] = fun.info.ID;
				data["target"] = funs.second[i].info.ID;
				data["Pearson"] = fun.corrs[i]; 
				
				edge["data"] = data;
				graph["elements"]["edges"].append(edge);
			}
		}
		return graph;
	}
}