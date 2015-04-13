#include "output_manager.hpp"

OutputManager::OutputManager(const bool _witness, const bool _robustness, const RegInfos & _reg_infos, const string & _name, sqlite3pp::database & _db)
	: witness(_witness), robustness(_robustness), reg_infos(_reg_infos), name(_name), db(_db)
{
	new_columns.insert({ "C_" + name, "INTEGER" });
	if (robustness)
		new_columns.insert({ "R_" + name, "REAL" });
	if (witness)
		new_columns.insert({ "W_" + name, "TEXT" });
}

void OutputManager::outputForm() {
	db.execute("BEGIN TRANSACTION;");
	for (const pair<string, string> & column : new_columns)
		sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, column.first, column.second, db);
	db.execute("END;");
}

// Output parametrizations from this round together with additional data, if requested.

void OutputManager::outputRound(const size_t cost, const double robustness_val, const string & witness_path, const Levels & parametrization, const ParamNo & rowid) {
	string command = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET ";
	command += "C_" + name + "=" + (cost == INF ? "NULL" : to_string(cost));
	if (robustness)
		command += ", R_" + name + "=" + to_string(robustness_val);
	if (witness)
		command += ", W_" + name + "=\"" + witness_path + "\"";
	command += " WHERE rowid=" + to_string(rowid);
	db.execute(command.c_str());
}
