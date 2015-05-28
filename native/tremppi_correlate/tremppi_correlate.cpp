#include <json/json.h>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/program_options.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/report/report.hpp>
#include "compute/compute.hpp"
#include "io/output.hpp"

// TODO: disable regulatory if not -r
int tremppi_correlate(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<ProgramOptions>("tremppi_function", argc, argv);
	Logging logging;

	Json::Value out;
	RegInfos reg_infos;
	sqlite3pp::database db;
	vector<sqlite3pp::query> queries;
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing data file.";
		// Read filter conditions
		out = Report::createSetup();

		db = move(sqlite3pp::database((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str()));

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
		FileManipulation::writeJSON(TremppiSystem::DATA_PATH / "correlate" / (out["setup"]["s_name"].asString() + ".json"), out);
	
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 5);
	}

	// TODO configure

	return 0;
}