/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "output_manager.hpp"

OutputManager::OutputManager(const bool _cost, const bool _trace, const bool _robustness, const RegInfos & _reg_infos, const string & _name, sqlite3pp::database & _db)
	: cost(_cost), trace(_trace), robustness(_robustness), reg_infos(_reg_infos), name(_name), db(_db)
{
	if (cost)
	{
		new_columns.insert({ "C_" + name, "INTEGER" });
	}
	if (robustness)
	{
		new_columns.insert({ "R_" + name, "REAL" });
	}
	if (_trace)
	{
		new_columns.insert({ "W_" + name, "TEXT" });

	}
}
void OutputManager::outputForm() 
{
	db.execute("BEGIN TRANSACTION;");
	for (const pair<string, string> & column : new_columns) 
	{
		sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, column.first, column.second, db);

	}
	db.execute("END;");
}

// Output parametrizations from this round together with additional data, if requested.
void OutputManager::outputRound(const size_t cost, const double robustness_val, const string & witness_path, const Levels & parametrization, const ParamNo & rowid) 
{
	string command = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET";
	if (cost) 
	{
		command += " C_" + name + "=" + (cost == INF ? "0" : to_string(cost)) + ",";
	}
	if (robustness)
	{
		command += " R_" + name + "=" + to_string(robustness_val) + ",";
	}
	if (trace)
	{
		command += " W_" + name + "=\"" + witness_path + "\"" + ",";
	}
	command.back() = ' ';
	command += "WHERE rowid=" + to_string(rowid);
	db.execute(command.c_str());
}
