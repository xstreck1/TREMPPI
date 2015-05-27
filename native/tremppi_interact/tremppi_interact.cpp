#include <json/json.h>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/report/report.hpp>
#include "compute/regulatory_graph.hpp"
#include "io/output.hpp"
#include "io/interact_options.hpp"

// TODO: disable regulatory if not -r
int tremppi_interact(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<InteractOptions>("tremppi_interact", argc, argv);
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

		BOOST_LOG_TRIVIAL(info) << "Computing interaction graph data.";

		logging.newPhase("Harvesting component", reg_infos.size());
		for (const RegInfo & reg_info : reg_infos) {
			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["select"].asString(), db);
			regs_data.emplace_back(RegulatoryGraph::build(reg_info, out["setup"]["size"].asInt(), sel_qry));
			// Get the statistics for the compare selection
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
		FileManipulation::writeJSON(TremppiSystem::DATA_PATH / "interact" / (TimeManager::getTimeStamp() + ".json"), out);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 5);
	}

	// TODO configure

	return 0;
}