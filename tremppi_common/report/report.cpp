#include "report.hpp"
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>

Json::Value Report::createSetup() {
	Json::Value out;

	out["setup"]["date"] = TimeManager::getTime();
	out["setup"]["name"] = TremppiSystem::WORK_PATH.stem().string();
	sqlite3pp::database db((TremppiSystem::WORK_PATH / DATABASE_FILENAME).string().c_str());

	out["setup"]["select"] = DatabaseReader::getSelectionTerm();
	// Get pool sizes
	out["setup"]["size"] = (sqlite3pp::query(db, ("SELECT COUNT(*) FROM " + PARAMETRIZATIONS_TABLE + DatabaseReader::getSelectionTerm()).c_str()).begin())->get<int>(0);

	return out;
}