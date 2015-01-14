#include "report.hpp"
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>

Json::Value Report::createSetup() {
	Json::Value out;

	out["setup"]["date"] = TimeManager::getTime();
	out["setup"]["name"] = tremppi_system.WORK_PATH.stem().string();
	sqlite3pp::database db((tremppi_system.WORK_PATH / DATABASE_FILENAME).string().c_str());

	out["setup"]["select"] = DatabaseReader::getSelectionTerm("Select");
	out["setup"]["compare"] = DatabaseReader::getSelectionTerm("Compare");
	// Comparing if the selecitons are not the same
	out["setup"]["comparative"] = out["setup"]["select"].asString() != out["setup"]["compare"].asString();

	// Get pool sizes
	out["setup"]["pool_size"] = (sqlite3pp::query(db, ("SELECT COUNT(*) FROM " + PARAMETRIZATIONS_TABLE).c_str()).begin())->get<int>(0);
	out["setup"]["selected"] = (sqlite3pp::query(db, ("SELECT COUNT(*) FROM " + PARAMETRIZATIONS_TABLE + " WHERE " + out["setup"]["select"].asString()).c_str()).begin())->get<int>(0);
	if (out["setup"]["comparative"].asBool())
		out["setup"]["compared"] = (sqlite3pp::query(db, ("SELECT COUNT(*) FROM " + PARAMETRIZATIONS_TABLE + " WHERE " + out["setup"]["compare"].asString()).c_str()).begin())->get<int>(0);
	else
		out["setup"]["compared"] = out["setup"]["pool_size"];

	return out;
}