#include <json/json.h>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/report/report.hpp>
#include "analysis/regulatory_graph.hpp"
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

		db = move(sqlite3pp::database((TremppiSystem::WORK_PATH / DATABASE_FILENAME).string().c_str()));

		// Copy the data
		FileManipulation::copyAnalysisFiles(TremppiSystem::WORK_PATH / ("interact_" + TimeManager::getTimeStamp()), "interact");

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	map<string, RegsData> data_types;
	try {


		BOOST_LOG_TRIVIAL(info) << "Computing interaction graph data.";
		data_types["select"] = RegsData();
		if (out["setup"]["comparative"].asBool()) {
			data_types["compare"] = RegsData();
			data_types["differ"] = RegsData();
		}

		logging.newPhase("Harvesting component", reg_infos.size());
		for (const RegInfo & reg_info : reg_infos) {
			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["select"].asString(), db);
			data_types["select"].emplace_back(RegulatoryGraph::build(reg_info, out["setup"]["selected"].asInt(), sel_qry));
			// Get the statistics for the compare selection
			if (out["setup"]["comparative"].asBool()) {
				sqlite3pp::query cmp_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["compare"].asString(), db);
				data_types["compare"].emplace_back(RegulatoryGraph::build(reg_info, out["setup"]["compared"].asInt(), cmp_qry));
			}
			logging.step();
		}
		// Compute difference
		if (out["setup"]["comparative"].asBool())
			data_types["differ"] = move(RegulatoryGraph::diff(data_types["select"], data_types["compare"]));

	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Building the JSON files.";
		// For each graph create the graph data and add configuration details
		for (auto & regs : data_types)
			out[regs.first] = Output::regulatoryGraph(reg_infos, regs);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Writing output.";
		// Write the computed content
		Json::StyledWriter writer;

		bfs::path output_path = TremppiSystem::WORK_PATH / ("interact_" + TimeManager::getTimeStamp() + ".json");
		fstream data_file(output_path.string(), ios::out);
		if (!data_file)
			throw runtime_error("Could not open " + output_path.string());

		string data = writer.write(out);
		data_file << data << endl;
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 5);
	}

	return 0;
}