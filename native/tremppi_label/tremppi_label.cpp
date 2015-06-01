#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/program_options.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/database/database_reader.hpp>

//
int tremppi_label(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<ProgramOptions>("tremppi_label", argc, argv);
	Logging logging;

	bfs::path database_path = TremppiSystem::DATA_PATH / DATABASE_FILENAME;
	string select;
	sqlite3pp::database db;
	try {
		// Get database
		db = move(sqlite3pp::database(database_path.string().c_str()));

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

	return 0;	
}