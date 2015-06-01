#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/program_options.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/database/database_reader.hpp>

#include "io/output.hpp"

//
int tremppi_label(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<ProgramOptions>("tremppi_label", argc, argv);
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
		Output::addColumns(reg_infos, db);

		logging.newPhase("Expressing component", reg_infos.size());

		for (const RegInfo & reg_info : reg_infos) {
			// For each context (indexed by ordinals, not IDs) hold the context that is obtained by removal of a specific regulator (indexed by ordinals, not IDs)
			vector<vector<size_t> > context_pairs;
			for (const auto & context : reg_info.contexts) {
				vector<size_t> pairs;
				for (const size_t reg_no : cscope(reg_info.regulators)) {
					// The threshold value is 0 => no removal is possible
					if (context.second[reg_no] == 0) {
						pairs.emplace_back(INF);
					}
					else {
						const CompID reg_ID = DataInfo::RegNoToRegID(reg_info, reg_no);
						const size_t column_ID = DataInfo::getColumnWithout(reg_info, context.first, reg_ID, context.second[reg_no]);
						pairs.emplace_back(DataInfo::columnIDToColumnNo(reg_info, column_ID));
					}
				}
			}

			// Select parametrizations and IDs
			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, select, db);
			sqlite3pp::query sel_IDs = DatabaseReader::selectionIDs(select, db);
			sqlite3pp::query::iterator sel_it = sel_qry.begin();

			db.execute("BEGIN TRANSACTION");

			// Go through parametrizations
			for (auto sel_ID : sel_IDs) {
				Levels params = sqlite3pp::func::getRow<ActLevel>(sel_it, 0, sel_qry.column_count());

				string value = "";
				int rowid = sel_ID.get<int>(0);
				string update = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET F_" + reg_info.name + "=" + value + " WHERE ROWID=" + to_string(rowid);
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

	return 0;	
}