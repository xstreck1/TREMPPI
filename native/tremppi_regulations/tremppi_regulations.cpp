#include <json/json.h>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/general/program_options.hpp>
#include <tremppi_common/report/report.hpp>
#include <tremppi_common/python/python_functions.hpp>

#include "compute/regulatory_graph.hpp"
#include "compute/edge_signs.hpp"
#include "io/output.hpp"

// TODO: disable regulatory if not -r
int tremppi_regulations(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<ProgramOptions>("tremppi_regulations", argc, argv);
	Logging logging;

	Json::Value out;
	RegInfos reg_infos;
	sqlite3pp::database db;
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing data file.";
		// Read filter conditions
		out = Report::createSetup();

		db = move(sqlite3pp::database((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str()));

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	RegsData regs_data;
	try {
		BOOST_LOG_TRIVIAL(info) << "Computing regulationsion graph data.";

		logging.newPhase("Harvesting component", reg_infos.size());
		for (const RegInfo & reg_info : reg_infos) {
			regs_data.emplace_back(RegData(reg_info));
			RegData & reg_data = regs_data.back();

			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["select"].asString(), db);
			RegulatoryGraph::compute(reg_info, out["setup"]["size"].asInt(), sel_qry, reg_data.reg_corr);

			EdgeSigns::computeExpectedFreq(reg_info, reg_data.expected_freq);

			map<size_t, string> label_columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("L_.*" + reg_info.name), db);
			if (!label_columns.empty()) {
				sqlite3pp::query label_qry = DatabaseReader::selectionFilter(label_columns, out["setup"]["select"].asString(), db);
				EdgeSigns::compute(reg_infos, reg_info.ID, out["setup"]["size"].asInt(), label_qry, reg_data.reg_freq, reg_data.reg_sign);
			}

			logging.step();
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Building the JSON files.";
		// For each graph create the graph data and add configuration details
		out["elements"] = Output::regulatoryGraph(reg_infos, regs_data);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Writing output.";
		FileManipulation::writeJSON(TremppiSystem::DATA_PATH / "regulations" / (out["setup"]["s_name"].asString() + ".json"), out);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 5);
	}

	try {
		PythonFunctions::configure("regulations");
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 6);
	}


	return 0;
}