#include <tremppi_common/general/system.hpp>
#include "compute/MVQMC.hpp"
#include "io/express_options.hpp"
#include "io/output.hpp"

//
int tremppi_express(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<ExpressOptions>("tremppi_express", argc, argv);
	Logging logging;

	bfs::path database_path = TremppiSystem::WORK_PATH / DATABASE_FILENAME;
	string select;
	map<string, ActLevel> maxes;
	RegFuncs functions;
	sqlite3pp::database db;
	try {
		// Get database
		db = move(sqlite3pp::database(database_path.string().c_str() ));

		select = DatabaseReader::getSelectionTerm();

		DatabaseReader reader;
		RegInfos infos = reader.readRegInfos(db);

		// Obtain the components data
		for (auto & info : infos) {
			Configurations minterms;
			for (const pair<size_t, string> column : info.columns)
				minterms.emplace_back(DataConv::getThrsFromContext(column.second));
			functions.emplace_back(RegFunc{ move(info), move(minterms) });
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	// Convert and output
	try {
		Output::addColumns(functions, db);

		logging.newPhase("Expressing component", functions.size());

		for (const RegFunc & reg_func : functions) {
			// Select parametrizations and IDs
			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_func.info.columns, select, db);
			sqlite3pp::query sel_IDs = DatabaseReader::selectionIDs(select, db);
			sqlite3pp::query::iterator sel_it = sel_qry.begin();

			db.execute("BEGIN TRANSACTION");

			// Go through parametrizations
            for (auto sel_ID : sel_IDs) {
				vector<vector<PMin>> config_values(reg_func.info.max_activity + 1);
				Levels params = sqlite3pp::func::getRow<ActLevel>(sel_it, 0, sel_qry.column_count());
				
				// Convert the contexts 
				for (const size_t context_id : cscope(params)) {
					PMin minterm(reg_func.minterms[context_id].size());
					transform(WHOLE(reg_func.minterms[context_id]), begin(minterm), [](ActLevel act_level){return PLit{ act_level }; });
					config_values[params[context_id]].emplace_back(minterm);
				}

				// Minimize for all values
				vector<PDNF> pdnfs(1);
				for (ActLevel target_val = 1; target_val < (reg_func.info.max_activity + 1); target_val++) {
					pdnfs.emplace_back(MVQMC::compactize(config_values[target_val]));
				}
				
				string formula = "\"" + Output::pdnfToFormula(functions, reg_func, pdnfs) + "\"";
				string update = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET F_" + reg_func.info.name + "=" + formula + " WHERE ROWID=" + to_string(sel_ID.get<int>(0));
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
