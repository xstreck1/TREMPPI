#include "report.hpp"
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>

Json::Value Report::createSetup() {
	Json::Value out;

	out["setup"]["date"] = TimeManager::getTime();
	sqlite3pp::database db((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str());

	out["setup"]["select"] = DatabaseReader::getSelectionTerm();
	out["setup"]["s_name"] = DatabaseReader::getSelectionName();
	// Get pool sizes
	out["setup"]["size"] = (sqlite3pp::query(db, ("SELECT COUNT(*) FROM " + PARAMETRIZATIONS_TABLE + DatabaseReader::getSelectionTerm()).c_str()).begin())->get<int>(0);

	return out;
}


string Report::reformName(string name) {
	switch (name[0]) {
		case 'B':
			return regex_replace(name, regex("C_(.*)"), "Bias($1)");
		case 'C':
			return regex_replace(name, regex("C_(.*)"), "Cost($1)");
		case 'E':
			return regex_replace(name, regex("E_(.*)"), "Indigree($1)");
		case 'F':
			return regex_replace(name, regex("F_(.*)"), "F<sub>$1</sub>");
		case 'I':
			return regex_replace(name, regex("I_(.*)_(\\d)_(.*)"), "Impact($1,$2,$3)");
		case 'K':
			return regex_replace(name, regex("K_(.*)_(\\d*)"), "K<sub>$1</sub>($2)");
		case 'S':
			return regex_replace(name, regex("S_(.*)_(\\d)_(.*)"), "Sign($1,$2,$3)");
		case 'R':
			return regex_replace(name, regex("R_(.*)"), "Robustness($1)");
	}
}