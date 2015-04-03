#pragma once

#include <tremppi_common/network/data_info.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Class that outputs formatted resulting data.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OutputManager {
	const bool witness; ///< Write the witness?
	const bool robustness; ///< Write the robustness?
	const RegInfos & reg_infos; ///< Reference to the model itself.
	const string & name; ///< Name of the property
	sqlite3pp::database & db;
	map<string, string> new_columns; ///< (column name, column type)

public:
	NO_COPY(OutputManager)

		// Store the names of the columns to be used
		OutputManager(const bool _witness, const bool _robustness, const RegInfos & _reg_infos, const string & _name, sqlite3pp::database & _db)
		: witness(_witness), robustness(_robustness), reg_infos(_reg_infos), name(_name), db(_db)
	{
		new_columns.insert({ "C_" + name, "INTEGER" });
		if (robustness)
			new_columns.insert({ "R_" + name, "REAL" });
		if (witness)
			new_columns.insert({ "W_" + name, "TEXT" });
	}

	// Write the new columns to the database
	void outputForm() {
		db.execute("BEGIN TRANSACTION;");
		for (const pair<string, string> & column : new_columns)
			sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, column.first, column.second, db);
		db.execute("END;");
	}

	// Output parametrizations from this round together with additional data, if requested.
	void outputRound(const size_t cost, const double robustness_val, const string & witness_path, const Levels & parametrization, const ParamNo & rowid) {
		string command = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET ";
		command += "C_" + name + "=" + (cost == INF ? "NULL" : to_string(cost));
		if (robustness)
			command += ", R_" + name + "=" + to_string(robustness_val);
		if (witness)
			command += ", W_" + name + "=\"" + witness_path + "\"";
		command += " WHERE rowid=" + to_string(rowid);
		db.execute(command.c_str());
	}
};