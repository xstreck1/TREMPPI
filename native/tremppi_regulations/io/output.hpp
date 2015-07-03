#pragma once

#include <tremppi_common/header.h>

#include "../data/data_storage.hpp"

namespace Output 
{
	Json::Value regulatoryGraph(const RegInfos & reg_infos, const RegsData & regs_data) {
		Json::Value elements;

		// Write the nodes
		string nodes;
		elements["nodes"].resize(0);
		for (const RegData & reg : regs_data) 
		{
			Json::Value node, data;
			data["id"] = reg._info.name;
			data["name"] = reg._info.name;
			node["data"] = data;
			elements["nodes"].append(node);
		}

		// Write the edges
		elements["edges"].resize(0);
		for (const RegData & reg : regs_data) 
		{
			size_t reg_i = 0;
			for (auto & regul : reg._info.regulators) 
			{
				for (const size_t trh_i : cscope(regul.second)) {
					// Skip those with no occurence
					if (abs(reg.reg_freq.at(regul.first)[trh_i]) != 0) 
					{
						Json::Value edge, data;

						data["source"] = reg_infos[regul.first].name;
						data["target"] = reg._info.name;
						data["Frequency"] = reg.reg_freq.at(regul.first)[trh_i];
						data["WeightedFrequency"] = reg.reg_freq.at(regul.first)[trh_i] / (reg.expected_freq.at(regul.first) * 2);
						data["Pearson"] = reg.reg_corr.at(regul.first)[trh_i];
						data["Threshold"] = regul.second[trh_i];
						data["Sign"] = string(1, reg.reg_sign.at(regul.first)[trh_i]);

						edge["data"] = data;
						elements["edges"].append(edge);
					}
					++reg_i;
				}
			}
		}
		return elements;
	}
}