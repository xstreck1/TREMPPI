#include "report.hpp"
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>

Json::Value Report::createSetup() {
	Json::Value out;

	out["setup"]["date"] = TimeManager::getTime();
	out["setup"]["model"] = TremppiSystem::WORK_PATH.stem().string();
	sqlite3pp::database db((TremppiSystem::WORK_PATH / DATABASE_FILENAME).string().c_str());

	out["setup"]["select"] = DatabaseReader::getSelectionTerm();
	out["setup"]["s_name"] = DatabaseReader::getSelectionName();
	// Get pool sizes
	out["setup"]["size"] = (sqlite3pp::query(db, ("SELECT COUNT(*) FROM " + PARAMETRIZATIONS_TABLE + DatabaseReader::getSelectionTerm()).c_str()).begin())->get<int>(0);

	return out;
}


string Report::reformName(string name) {
	if (count(WHOLE(name), '_') == 1) {
		name += "_";
	}
	else if (count(WHOLE(name), '_') < 1) {
		throw runtime_error("reforming a name did not work since there's only one underscore");
	}
	name.replace(name.find("_"), 1, "<sub>");
	name.replace(name.find("_"), 1, "</sub>(");
	name.append(")");
	return name;
}