#include <json/json.h>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/report/report.hpp>
#include "analysis/statistical_analysis.hpp"
#include "io/output.hpp"
#include "io/interact_options.hpp"

// TODO: disable regulatory if not -r
int tremppi_function(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<FunctionOptions>("tremppi_function", argc, argv);
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
		FileManipulation::copyAnalysisFiles(TremppiSystem::WORK_PATH / ("function_" + TimeManager::getTimeStamp()), "function");

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	map<string, FunsData> data_types;
	try {


		BOOST_LOG_TRIVIAL(info) << "Computing interaction graph data.";
		data_types["select"] = FunsData();
		if (out["setup"]["comparative"].asBool()) {
			data_types["compare"] = FunsData();
			data_types["differ"] = FunsData();
		}

		logging.newPhase("Harvesting component", reg_infos.size());
		for (const RegInfo & reg_info : reg_infos) {
			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["select"].asString(), db);
			data_types["select"].emplace_back(StatisticalAnalysis::build(reg_info, out["setup"]["selected"].asInt(), sel_qry));
			// Get the statistics for the compare selection
			if (out["setup"]["comparative"].asBool()) {
				sqlite3pp::query com_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["compare"].asString(), db);
				data_types["compare"].emplace_back(StatisticalAnalysis::build(reg_info, out["setup"]["compared"].asInt(), com_qry));
			}
			logging.step();
		}
		// Compute difference
		if (out["setup"]["comparative"].asBool())
			data_types["differ"] = move(StatisticalAnalysis::diff(data_types["select"], data_types["compare"]));

	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Building the JSON files.";
		// For each graph create the graph data and add configuration details
		for (auto & funs : data_types)
			out[funs.first] = Output::functionalData(funs);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Writing output.";
		// Write the computed content
		Json::StyledWriter writer;

		bfs::path output_path = TremppiSystem::WORK_PATH / ("function" + TimeManager::getTimeStamp() + ".json");
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