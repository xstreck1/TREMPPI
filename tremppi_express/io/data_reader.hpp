#pragma once

#include <tremppi_common/database/database_reader.hpp>

#include "../data/data.hpp"

namespace DataReader {
	// Read headers for all the regulatory functions
	RegFuncs readRegInfo(const sqlite3pp::database & db) {
		RegFuncs result;

		// Read common reg_data
		vector<string> names = db.readColumn<string>(COMPONENTS_TABLE, DatabaseReader::NAMES_COLUMN, "");
		for (const string name : names) {
			regex prefix("K_" + name + ".*");
			size_t ID = result.size();
			ActLevel max = DatabaseReader::getMaxLevel(name, database, "");
			auto regulators = DatabaseReader::obtainRegulators(name, database);
			auto columns = database.getColumnsByExpression(PARAMETRIZATIONS_TABLE, prefix);

			result.emplace_back(RegFunc{ ID, name, max, regulators, columns.first, columns.second });
		}

		return result;
	}
}