#pragma once
#include "../data/data_storage.hpp"

namespace Output {
	Json::Value functionalData(const FunsData & funs_data) {
		Json::Value elements;

		// Write the nodes
		string nodes;
		elements["nodes"].resize(0);
		for (const FunData & fun : funs_data) {
			Json::Value node, data;
			data["id"] = fun.info.name;
			data["name"] = fun.info.name;
			data["Bias"] = ((fun.mean * 2) - fun.info.max_activity) / fun.info.max_activity;
			data["Mean"] = fun.mean;
			data["Std"] = fun.std_dev;
			data["Max"] = fun.info.max_activity;

			node["data"] = data;
			elements["nodes"].append(node);
		}

		// Write the edges
		elements["edges"].resize(0);
		for (const FunData & fun : funs_data) {
			size_t reg_i = 0;
			for (const int i : cscope(fun.corrs)) {
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