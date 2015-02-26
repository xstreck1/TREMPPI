#include <json/json.h>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/report/report.hpp>
#include "analysis/compute.hpp"
#include "io/output.hpp"
#include "io/function_options.hpp"

// TODO: disable regulatory if not -r
int tremppi_function(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<FunctionOptions>("tremppi_function", argc, argv);
	Logging logging;

	Json::Value out;
	RegInfos reg_infos;
	sqlite3pp::database db;
	map<string, vector<sqlite3pp::query>> queries;
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
		for (const RegInfo & reg_info : reg_infos) {
			queries["select"].emplace_back(DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["select"].asString(), db));
			IF_CMP queries["compare"].emplace_back(DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["compare"].asString(), db));
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	map<string, FunsData> data_types;
	try {
		BOOST_LOG_TRIVIAL(info) << "Computing function graph data.";
		data_types["select"] = FunsData();
		IF_CMP data_types["compare"] = FunsData();
		IF_CMP data_types["differ"] = FunsData();

		Compute::deviation(reg_infos, out["setup"]["selected"].asInt(), queries["select"], logging, data_types["select"]);
		IF_CMP Compute::deviation(reg_infos, out["setup"]["compared"].asInt(), queries["compare"], logging, data_types["compare"]);

		Compute::correlation(reg_infos, out["setup"]["selected"].asInt(), queries["select"], logging, data_types["select"]);
		IF_CMP Compute::correlation(reg_infos, out["setup"]["compared"].asInt(), queries["compare"], logging, data_types["compare"]);

		// Compute difference
		IF_CMP data_types["differ"] = move(StatisticalAnalysis::diff(data_types["select"], data_types["compare"]));
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

		bfs::path output_path = TremppiSystem::WORK_PATH / ("function_" + TimeManager::getTimeStamp() + ".json");
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