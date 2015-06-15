#include <json/json.h>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/program_options.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/report/report.hpp>
#include <tremppi_common/python/python_functions.hpp>

#include "compute/compute.hpp"
#include "io/output.hpp"

int tremppi_bias(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<ProgramOptions>("tremppi_bias", argc, argv);
	Logging logging;

	string select;
	sqlite3pp::database db;
	RegInfos reg_infos;
	try {
		// Get database
		db = move(sqlite3pp::database((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str()));

		select = DatabaseReader::getSelectionTerm();

		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	// Label per parametrization
	FunsData funs_data;
	try {
		BOOST_LOG_TRIVIAL(info) << "Computing function graph data.";
		// Add columns
		for (const RegInfo & reg_info : reg_infos) {
			const string column_name = "B_" + reg_info.name;
			sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, column_name, "REAL", db);
		}

		logging.newPhase("Bias of a component", reg_infos.size());

		for (const RegInfo & reg_info : reg_infos) {
			// Select parametrizations and IDs
			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, select, db);
			sqlite3pp::query sel_IDs = DatabaseReader::selectionIDs(select, db);
			sqlite3pp::query::iterator sel_it = sel_qry.begin();

			db.execute("BEGIN TRANSACTION");

			// Go through parametrizations
			for (auto sel_ID : sel_IDs) {
				Levels params = sqlite3pp::func::getRow<ActLevel>(*sel_it, 0, sel_qry.column_count());
				string update = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET";
				double bias = Statistics::expected_val(params) / reg_info.max_activity;
				// Remove the last comma
				update += " B_" + reg_info.name + " = " + to_string(bias);
				int rowid = sel_ID.get<int>(0);
				update += " WHERE ROWID=" + to_string(rowid);
				db.execute(update.c_str());

				sel_it++;
			}
			db.execute("END");

			logging.step();
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	try {
		PythonFunctions::configure("select");
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}

	return 0;
}

int tremppi_correlations(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<ProgramOptions>("tremppi_correlations", argc, argv);
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
			map<size_t, string> columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("B_" + reg_info.name), db);
			if (columns.empty()) {
				throw runtime_error("did not find the column B_" + reg_info.name);
			}
			queries.emplace_back(DatabaseReader::selectionFilter(columns, out["setup"]["select"].asString(), db));
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
		FileManipulation::writeJSON(TremppiSystem::DATA_PATH / "bias" / (out["setup"]["s_name"].asString() + ".json"), out);

	}
	catch (exception & e) {
		logging.exceptionMessage(e, 5);
	}


	try {
		PythonFunctions::configure("bias");
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 6);
	}


	return 0;
}