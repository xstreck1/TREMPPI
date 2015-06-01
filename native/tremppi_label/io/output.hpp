#pragma once

#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/database/database_reader.hpp>

#include "../data/data.hpp"

namespace Output {
	// Add a function column for each component 
	void addColumns(const RegInfos & reg_infos, sqlite3pp::database & db) {
		for (const RegInfo & reg_info : reg_infos) {
			sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, "L_" + reg_info.name, "STRING", db);
		}
	}
}