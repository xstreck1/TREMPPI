#include <tremppi_common/general/logging.hpp>
#include "compute/MVQMC.hpp"
#include "io/program_options.hpp"
#include "io/output.hpp"

using namespace std;

// 
void exceptionMessage(const exception & e, const int err_no) {
	BOOST_LOG_TRIVIAL(error) << "Top level exception: " << e.what();
	exit(err_no);
}

//
int main(int argc, char ** argv) {
	Logging::init(PROGRAM_NAME + ".log");
	BOOST_LOG_TRIVIAL(info) << PROGRAM_NAME << " has started.";

	bpo::variables_map po;
	bfs::path input_path;
	string select;

	map<string, ActLevel> maxes;
	RegFuncs functions;
	sqlite3pp::database db;
	
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing data file.";
		// Get user options
		po = ProgramOptions::parseProgramOptions(argc, argv);
		input_path = ProgramOptions::getDatabasePath(po);

		// Get database
		db = move(database(po["database"].as<string>().c_str()));

		// Read filter conditions
		if (po.count("select") > 0)
			select = ProgramOptions::getFilter(po["select"].as<string>());

		// Read regulatory information
		query qry(db, ("SELECT * FROM " + COMPONENTS_TABLE).c_str());

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
		Logging::exceptionMessage(e, 1);
	}

	// Convert and output
	try {
		Output::addColumns(functions, db);

		for (const RegFunc & reg_func : functions) {
			// For an input function
			if (reg_func.info.regulators.empty()){
				db.execute(("UPDATE " + PARAMETRIZATIONS_TABLE + " SET F_" + reg_func.info.name + "=" + quote("const")).c_str());
				continue;
			}

			query sel_qry = DatabaseReader::selectionFilter(reg_func.info.columns, select, db);
			query sel_IDs = DatabaseReader::selectionIDs(select, db);
			sqlite3pp::query::iterator sel_it = sel_qry.begin();

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
		}
	}
	catch (exception & e) {
		Logging::exceptionMessage(e, 2);
	}

	BOOST_LOG_TRIVIAL(info) << PROGRAM_NAME << " finished successfully.";
	return 0;
}
