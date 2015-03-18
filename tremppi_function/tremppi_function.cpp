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
	vector<sqlite3pp::query> queries;
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing data file.";
		// Read filter conditions
		out = Report::createSetup();

		db = move(sqlite3pp::database((TremppiSystem::WORK_PATH / DATABASE_FILENAME).string().c_str()));

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
		for (const RegInfo & reg_info : reg_infos) {
			queries.emplace_back(DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["select"].asString(), db));
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	FunsData funs_data;
	try {
		BOOST_LOG_TRIVIAL(info) << "Computing function graph data.";

		Compute::deviation(reg_infos, out["setup"]["size"].asInt(), queries, logging, funs_data);

		Compute::correlation(reg_infos, out["setup"]["size"].asInt(), queries, logging, funs_data);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Building the JSON.";
		out["elements"] = Output::functionalData(funs_data);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Writing output.";
		FileManipulation::writeJSON(TremppiSystem::WORK_PATH / "function" / (TimeManager::getTimeStamp() + ".json"), out);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 5);
	}

	return 0;
}