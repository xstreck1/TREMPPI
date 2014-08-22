#pragma once

#include <tremppi_common/header.h>

#include "../data/data_storage.hpp"

namespace Output {
	const string STABILITY_COLUMN = "Stability";
	const string COMPONENT_NO_COLUMN = "Component_no";
	const string INT_TYPE = "int";
	const string REGULATORY = "regulatory";

	inline string styleAtt(const string & values) {
		return " style=\"" + values + "\"";
	}

	inline string classAtt(const string & values) {
		return " class=\"" + values + "\"";
	}

	inline string addPars(const string & source) {
		return "\"" + source + "\"";
	}

	//
	/*void updateDatabase(const Par & par, SQLAdapter & database) {
		database.safeExec("BEGIN TRANSACTION;");
		database.prepareColumn(PARAMETRIZATIONS_TABLE, STABILITY_COLUMN, INT_TYPE);
		database.prepareColumn(PARAMETRIZATIONS_TABLE, COMPONENT_NO_COLUMN, INT_TYPE);
		database.updateColumn(PARAMETRIZATIONS_TABLE, STABILITY_COLUMN, par.neigh_count, par.row_ids);
		database.updateColumn(PARAMETRIZATIONS_TABLE, COMPONENT_NO_COLUMN, par.component_membership, par.row_ids);
		database.safeExec("COMMIT;");
		}*/

	//
	/*void correlationGraph(const Regs & regs, bfs::path original_path, const string & type) {
		bfs::path output_path = original_path.parent_path() /= bfs::path{ original_path.stem().string() + "_functions_" + type + GRAPH_SUFFIX };
		ofstream file(output_path.string(), ios::out);

		file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?> \n"
		<< "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
		<< "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
		<< "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns http ://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> \n";

		file << "\t <graph id=\"" << original_path.filename().string() << "_functions\" edgedefault=\"undirected\"> \n";
		file << "\t\t <key id=\"std_dev\" for=\"node\" attr.name=\"standard deviation\" attr.type=\"double\"/> \n";
		file << "\t\t <key id=\"dev_sign\" for=\"node\" attr.name=\"sign of the deviation\" attr.type=\"string\"/> \n";
		file << "\t\t <key id=\"unique\" for=\"node\" attr.name=\"number of unique parametrizations\" attr.type=\"double\"/> \n";
		file << "\t\t <key id=\"choice\" for=\"node\" attr.name=\"proportion of options\" attr.type=\"double\"/> \n";
		file << "\t\t <key id=\"mean\" for=\"node\" attr.name=\"mean parameter value\" attr.type=\"double\"/> \n";
		file << "\t\t <key id=\"pearson\" for=\"edge\" attr.name=\"correlation coefficient\" attr.type=\"double\"/> \n";

		for (const Reg & reg : regs) {
		file << "\t\t <node id=\"" << reg.name << "\"> \n";
		file << "\t\t\t <data key=\"mean\">" << reg.mean << "</data> \n";
		file << "\t\t\t <data key=\"std_dev\">" << abs(reg.std_dev) << "</data> \n";
		file << "\t\t\t <data key=\"dev_sign\">" << (signbit(reg.std_dev) ? "-" : "+") << "</data> \n";
		double choice_norm = (reg.choice + regs.size()) / regs.size();
		file << "\t\t\t <data key=\"unique\">" << reg.unique.size() << "</data> \n";
		file << "\t\t\t <data key=\"choice\">" << reg.choice << "</data> \n";
		file << "\t\t </node> \n";
		}

		for (const Reg & reg : regs) {
		for (const size_t o_ID : crange(reg.ID)) {
		if (abs(reg.correlations[o_ID]) == 0)
		continue;

		file << "\t\t <edge  id=\"" << regs[o_ID].name << "_" << reg.name << "\" "
		<< "source=\"" << regs[o_ID].name << "\" "
		<< "target=\"" << reg.name << "\" "
		<< ">\n";
		file << "\t\t\t <data key=\"pearson\">" << reg.correlations[o_ID] << "</data> \n";
		file << "\t\t </edge> \n";
		}
		}

		file << "\t </graph> \n";

		file << "</graphml> \n";
		}*/

	Json::Value regulatoryGraph(const pair<string, RegsData> & regs) {
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
				// Skip those with no occurence
				if (abs(reg.reg_freq[reg_i]) != 0 || regs.first == "differ") {
					Json::Value edge, data;

					data["source"] = regul.first;
					data["target"] = reg.info.name;
					data["pearson"] = reg.reg_corr[reg_i];
					data["frequency"] = reg.reg_freq[reg_i];

					edge["data"] = data;
					graph["elements"]["edges"].append(edge);
				}
				++reg_i;
			}
		}
		return graph;
	}

	Json::Value functionalData(const pair<string, FunsData> & funs) {
		Json::Value graph;

		return graph;
	}

	//
	/*void plas(const Regs & regs, bfs::path original_path, const string & type) {
		for (const Reg & reg : regs) {
		bfs::path output_path = original_path.parent_path() /= bfs::path{ original_path.stem().string() + "_" + reg.name + "_" + type + PLA_SUFFIX };
		ofstream file(output_path.string(), ios::out);

		file << ".i " << reg.params_ids.size() << endl;
		file << ".o " << "1" << endl;
		// file << ".ilb";

		for (const Levels & param : reg.unique) {
		for (const ActLevel & val : param) {
		file << val;
		}
		file << " 1\n";
		}

		file << ".end" << endl;
		}
		}*/

	//
	void copyReport(const bfs::path & input_path) {
		// Copy the data from "../Files/Standine_report"
		bfs::path report_dir = input_path.parent_path() / (input_path.stem().string() + "_report");
		if (bfs::exists(report_dir))
			bfs::remove_all(report_dir);
		FileManipulation::copyDir(tremppi_system.HOME_PATH / bfs::path{ "javascript" } / bfs::path{ "report" }, report_dir);

		// Delete the original models
		vector<string> to_delete = { "data.js", "configure.json", "regulatory_select.json", "regulatory_differ.json", "regulatory_compare.json" };
		for_each(WHOLE(to_delete), [&report_dir](const string & filename) {
			bfs::remove(report_dir / bfs::path{ filename });
		});
	}

	//
	ofstream fileOutput(const bfs::path & input_path, const string & name) {
		bfs::path output_path = input_path.parent_path() /= bfs::path{ input_path.stem().string() + "_report" } /= name;
		ofstream file = ofstream(output_path.string(), ios::out);
		if (!file)
			throw runtime_error("Could not open " + output_path.string());
		return file;
	}
}

