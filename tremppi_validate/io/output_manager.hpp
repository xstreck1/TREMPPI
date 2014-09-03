#pragma once

#include <tremppi_common/network/data_info.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Class that outputs formatted resulting data.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OutputManager {
	const bpo::variables_map & po; ///< User can influence the format of the output.
	const RegInfos & reg_infos; ///< Reference to the model itself.
	const string & name; ///< Name of the property
	sqlite3pp::database & db;
	map<string, string> new_columns; ///< (column name, column type)

public:
	NO_COPY(OutputManager)

	// Store the names of the columns to be used
	OutputManager(const bpo::variables_map  & _po, const RegInfos & _reg_infos, const string & _name, sqlite3pp::database & _db)
		: po(_po), reg_infos(_reg_infos), name(_name), db(_db) 
	{
		new_columns.insert({ "C_" + name, "INTEGER" });
		if (po.at("trace").as<string>() == "rob" || po.at("trace").as<string>() == "wit")
			new_columns.insert({ "R_" + name, "REAL" });
		if ( po.at("trace").as<string>() == "wit")
			new_columns.insert({ "W_" + name, "TEXT" });
	}

	// Write the new columns to the database
	void outputForm() {
		for (const pair<string, string> & column : new_columns)
			sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, column.first, column.second, db);
	}

	// Output parametrizations from this round together with additional data, if requested.
	void outputRound(const size_t cost, const double robustness_val, const string & witness, const Levels & parametrization, const ParamNo & rowid) {
		string command = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET ";
		command += "C_" + name + "=" + (cost == INF ? "NULL" : to_string(cost));
		if (po.at("trace").as<string>() == "rob" || po.at("trace").as<string>() == "wit")
			command += ", R_" + name + "=" + to_string(robustness_val);
		if (po.at("trace").as<string>() == "wit")
			command += ", W_" + name + "=" + witness;
		command += " WHERE rowid=" + to_string(rowid);
		db.execute(command.c_str());
	}
};