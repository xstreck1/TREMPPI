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
int tremppi_impact(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<ProgramOptions>("tremppi_impact", argc, argv);
	Logging logging;

	bfs::path database_path = TremppiSystem::DATA_PATH / DATABASE_FILENAME;
	string select;
	sqlite3pp::database db;
	RegInfos reg_infos;
	try {
		// Get database
		db = move(sqlite3pp::database(database_path.string().c_str()));

		select = DatabaseReader::getSelectionTerm();

		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	// Label per parametrization
	try {
		for (const RegInfo & reg_info : reg_infos) {
			for (const auto & regulator : reg_info.regulators) {
				const string reg_name = reg_infos[regulator.first].name;
				for (const ActLevel & threshold : regulator.second) {
					const string column_name = "I_" + reg_name + "_" + to_string(threshold) + "_" + reg_info.name;
					sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, column_name, "REAL", db);
				}
			}
		}

		logging.newPhase("Expressing component", reg_infos.size());

		for (const RegInfo & reg_info : reg_infos) {
			// Skip if there are no edges
			if (reg_info.regulators.empty()) {
				continue;
			}

			// Get the header data
			vector<string> columns_list = DataConv::columns2list(reg_info.columns);
			// For each regulator holds the values of the threshold in each column
			vector<Levels> reg_values;
			// For each regulator hold the columns that contain the contexts with this regulator and those exactly without is (i.e. if I have a context with this regulator and remove the regulator, what do I obtain?)
			map<CompID, vector<vector<size_t>>> edge_dist;
			for (const auto & regulator : reg_info.regulators) {
				// Obtain the id of the regulator from the order in the regulator list
				size_t reg_i = DataInfo::RegIDToRegNo(reg_info, regulator.first);
				reg_values.emplace_back(DataConv::getThrFromContexts(columns_list, reg_i));
				edge_dist.insert({ regulator.first, RegulatoryGraph::getColumnsOfRegulator(reg_i, regulator.second, reg_info.columns) });
			}

			// Select parametrizations and IDs
			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, select, db);
			sqlite3pp::query sel_IDs = DatabaseReader::selectionIDs(select, db);
			sqlite3pp::query::iterator sel_it = sel_qry.begin();

			db.execute("BEGIN TRANSACTION");

			// Go through parametrizations
			for (auto sel_ID : sel_IDs) {
				string update = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET";
				Levels params = sqlite3pp::func::getRow<ActLevel>(*sel_it, 0, sel_qry.column_count());

				for (const size_t reg_no : cscope(reg_info.regulators)) {
					const CompID reg_ID = DataInfo::RegNoToRegID(reg_info, reg_no);
					for (const size_t threshold_no : cscope(reg_info.regulators.at(reg_ID))) {
						const ActLevel threshold = reg_info.regulators.at(reg_ID)[threshold_no];
						const double impact = Statistics::correlation(reg_values[reg_no], params, edge_dist.at(reg_ID)[threshold_no], reg_info.columns.begin()->first);
						update += " I_" + reg_infos[reg_ID].name + "_" + to_string(threshold) + "_" + reg_info.name + " = " + to_string(impact) + ",";
					}
				}
				update[update.size() - 1] = ' ';
				int rowid = sel_ID.get<int>(0);
				update += "WHERE ROWID=" + to_string(rowid);
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

int tremppi_regulations(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<ProgramOptions>("tremppi_regulations", argc, argv);
	Logging logging;

	Json::Value out;
	RegInfos reg_infos;
	sqlite3pp::database db;
	try {
		DEBUG_LOG << "Parsing data file.";
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
		DEBUG_LOG << "Computing regulationsion graph data.";

		logging.newPhase("Harvesting component", reg_infos.size());
		for (const RegInfo & reg_info : reg_infos) {
			regs_data.emplace_back(RegData(reg_info));
			RegData & reg_data = regs_data.back();

			map<size_t, string> impact_columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("I_.*" + reg_info.name), db);
			if (!impact_columns.empty()) {
				sqlite3pp::query impact_qry = DatabaseReader::selectionFilter(impact_columns, out["setup"]["select"].asString(), db);
				RegulatoryGraph::compute(reg_infos, reg_info.ID, out["setup"]["size"].asInt(), impact_qry, reg_data.reg_corr);
			}

			EdgeSigns::computeExpectedFreq(reg_info, reg_data.expected_freq);

			map<size_t, string> sign_columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("S_.*" + reg_info.name), db);
			if (!sign_columns.empty()) {
				sqlite3pp::query label_qry = DatabaseReader::selectionFilter(sign_columns, out["setup"]["select"].asString(), db);
				EdgeSigns::compute(reg_infos, reg_info.ID, out["setup"]["size"].asInt(), label_qry, reg_data.reg_freq, reg_data.reg_sign);
			}

			logging.step();
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	try {
		DEBUG_LOG << "Building the JSON files.";
		// For each graph create the graph data and add configuration details
		out["elements"] = Output::regulatoryGraph(reg_infos, regs_data);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}

	try {
		DEBUG_LOG << "Writing output.";
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