#include <tremppi_common/general/logging.hpp>
#include "compute/MVQMC.hpp"
#include "io/program_options.hpp"
#include "io/output.hpp"

using namespace TremppiExpress;

//
int tremppi_express(int argc, char ** argv) {
	bpo::variables_map po; // program options provided on the command line
	bfs::path input_path; // an input path

	try {
		if (argc < 1)
			throw runtime_error("No parameters.");

		po = ProgramOptions::parseProgramOptions(argc, argv);
		input_path = ProgramOptions::getDatabasePath(po);

		tremppi_system.set("tremppi_express", argv[0], input_path.parent_path());
		logging.phase_count = 1;
		logging.init(tremppi_system.PROGRAM_NAME + ".log");
		BOOST_LOG_TRIVIAL(info) << "TREMPPI expression minimizer (" << tremppi_system.PROGRAM_NAME << ") started.";
	}
	catch (exception & e) {
		cerr << e.what();
		return 1;
	}

	string select;
	map<string, ActLevel> maxes;
	RegFuncs functions;
	sqlite3pp::database db;
	try {
		// Get database
		db = move(sqlite3pp::database(po["database"].as<string>().c_str()));

		// Read filter conditions
		if (po.count("select") > 0)
			select = ProgramOptions::getFilter(po["select"].as<string>());

		// Read regulatory information
		sqlite3pp::query qry(db, ("SELECT * FROM " + COMPONENTS_TABLE).c_str());

		// Obtain the components data
		for (auto row : qry) {
			string name; ActLevel max;
			row.getter() >> name >> max;
			maxes.insert(make_pair(name, max));
			RegInfo info = DatabaseReader::readRegInfo(name, db);
			Configurations minterms;
			for (const pair<string, size_t> column : info.columns)
				minterms.emplace_back(DatabaseReader::getThrsFromContext(column.first));
			functions.emplace_back(RegFunc{ move(info), move(minterms) });
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 1);
	}

	// Convert and output
	try {
		Output::addColumns(functions, db);

		for (const RegFunc & reg_func : functions) {
			// Select parametrizations and IDs
			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_func.info.columns, select, db);
			sqlite3pp::query sel_IDs = DatabaseReader::selectionIDs(select, db);
			sqlite3pp::query::iterator sel_it = sel_qry.begin();

			db.execute("BEGIN TRANSACTION");
			// Go through parametrizations
			for (auto & sel_ID : sel_IDs) {
				vector<vector<PMin>> config_values(reg_func.info.max + 1);
				Levels params = sqlite3pp::func::getRow<ActLevel>(sel_it, sel_qry.column_count());
				
				// Convert the contexts 
				for (const size_t context_id : cscope(params)) {
					PMin minterm(reg_func.minterms[context_id].size());
					transform(WHOLE(reg_func.minterms[context_id]), begin(minterm), [](ActLevel act_level){return PLit{ act_level }; });
					config_values[params[context_id]].emplace_back(minterm);
				}

				// Minimize for all values
				vector<PDNF> pdnfs(1);
				for (ActLevel target_val = 1; target_val < (reg_func.info.max + 1); target_val++) {
					pdnfs.emplace_back(MVQMC::compactize(config_values[target_val]));
				}
				
				string formula = "\"" + Output::pdnfToFormula(maxes, reg_func, pdnfs) + "\"";
				string update = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET F_" + reg_func.info.name + "=" + formula + " WHERE ROWID=" + to_string(sel_ID.get<int>(0));
				db.execute(update.c_str());

				sel_it++;
			}
			db.execute("END");
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	BOOST_LOG_TRIVIAL(info) << tremppi_system.PROGRAM_NAME << " finished successfully.";
	return 0;
}
