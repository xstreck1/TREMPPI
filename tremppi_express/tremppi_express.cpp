#include <tremppi_common/general/logging.hpp>
#include "compute/MVQMC.hpp"
#include "io/program_options.hpp"
#include "io/data_reader.hpp"
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
	return 0;

	bpo::variables_map po;
	sqlite3pp::database db;
	bfs::path input_path;
	RegFuncs reg_funcs;
	map<string, ActLevel> maxes;

	// parse input
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing data file.";
		po = parseProgramOptions(argc, argv);

		// Get database
		input_path = DatabaseReader::getDatabasePath(po);
		db.setDatabase(po["database"].as<string>());
		reg_funcs = DataReader::readRegInfo(db);

		for (RegFunc & reg_func : reg_funcs) {
			for (const string & context : reg_func.columns_name)  {
				reg_func.columns_minterm.emplace_back(DatabaseReader::getThrsFromContext(context));
			}
			maxes[reg_func.name] = reg_func.max;
		}

	}
	catch (exception & e) {
		Logging::exceptionMessage(e, 1);
	}

	// Convert and output
	try {
		Output::addColumns(reg_funcs, db);
		db.safeExec("BEGIN TRANSACTION;");

		for (const RegFunc & reg_func : reg_funcs) {
			db.accessTable(PARAMETRIZATIONS_TABLE);
			Levels params = db.getRow<ActLevel>(reg_func.columns_ID);
			// Go through parametrizations
			for (size_t ID = 1; !params.empty(); ID++, params = db.getRow<ActLevel>(reg_func.columns_ID)) {

				vector<vector<PMin>> config_values(reg_func.max + 1);
				// Convert the contexts 
				for (const size_t context_id : cscope(params)) {
					PMin minterm(reg_func.columns_minterm[context_id].size());
					transform(WHOLE(reg_func.columns_minterm[context_id]), begin(minterm), [](ActLevel act_level){return PLit{ act_level }; });
					config_values[params[context_id]].emplace_back(minterm);
				}

				// Minimize for all values
				vector<PDNF> pdnfs(1);
				for (ActLevel target_val = 1; target_val < (reg_func.max + 1); target_val++) {
					pdnfs.emplace_back(MVQMC::compactize(config_values[target_val]));
				}
				
				string formula = "\"" + Output::pdnfToFormula(maxes, reg_func, pdnfs) + "\"";
				db.updateColumn(PARAMETRIZATIONS_TABLE, "F_" + reg_func.name, formula, ID);
			}


		}
		db.safeExec("END TRANSACTION;");
	}
	catch (exception & e) {
		Logging::exceptionMessage(e, 2);
	}

	BOOST_LOG_TRIVIAL(info) << PROGRAM_NAME << " finished successfully.";
	return 0;
}
