#pragma once

#include <tremppi_common/header.h>

#include "../data/data_storage.hpp"

namespace Output {
	Json::Value regulatoryGraph(const RegInfos & reg_infos, const pair<string, RegsData> & regs) {
		Json::Value graph;

		// Write the nodes
		string nodes;
		for (const RegData & reg : regs.second) {
			Json::Value node, data;
			data["id"] = reg.info.name;
			data["name"] = reg.info.name;
			node["data"] = data;
			graph["elements"]["nodes"].append(node);
		}

		// Write the edges
		for (const RegData & reg : regs.second) {
			size_t reg_i = 0;
			for (auto & regul : reg.info.regulators) {
				for (const size_t trh_i : cscope(regul.second)) {
					// Skip those with no occurence
					if (abs(reg.reg_freq.at(regul.first)[trh_i]) != 0 || regs.first == "differ") {
						Json::Value edge, data;

						data["source"] = reg_infos[regul.first].name;
						data["target"] = reg.info.name;
						data["Pearson"] = reg.reg_corr.at(regul.first)[trh_i];
						data["Frequency"] = reg.reg_freq.at(regul.first)[trh_i];
						data["ExpectedFreq"] = reg.expected_freq.at(regul.first);
						data["Threshold"] = regul.second[trh_i];

						edge["data"] = data;
						graph["elements"]["edges"].append(edge);
					}
					++reg_i;
				}
			}
		}
		return graph;
	}
}