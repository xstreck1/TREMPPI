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

#include <json/json.h>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/python/python_functions.hpp>
#include <tremppi_common/report/statistics.hpp>

int tremppi_bias(int argc, char ** argv) 
{
	TremppiSystem::initiate("tremppi_bias", argc, argv);
	Logging logging;

	string selection;
	sqlite3pp::database db;
	RegInfos reg_infos;

	try 
	{
		// Get database
		db = move(sqlite3pp::database((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str()));
		// Get selection (use empty on server to prevent errors by insufficient data on the server)
		selection = TremppiSystem::called_from_server ? "" : DatabaseReader::getSelectionTerm();

		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
	}
	catch (exception & e) 
	{
		logging.exceptionMessage(e, 2);
	}

	// Label per parametrization
	try 
	{
		DEBUG_LOG << "Computing function graph data.";
		// Add columns
		for (const RegInfo & reg_info : reg_infos) 
		{
			const string column_name = "B_" + reg_info.name;
			sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, column_name, "REAL", db);
		}
		logging.newPhase("Bias of a component", reg_infos.size());

		for (const RegInfo & reg_info : reg_infos) 
		{
			// Select parametrizations and IDs
			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, selection, db);
			sqlite3pp::query sel_IDs = DatabaseReader::selectionIDs(selection, db);
			sqlite3pp::query::iterator sel_it = sel_qry.begin();

			logging.newPhase("Listing parametrizations", DatabaseReader::getSelectionSize(selection, db));
			db.execute("BEGIN TRANSACTION");
			// Go through parametrizations
			for (auto sel_ID : sel_IDs) 
			{
				Levels params = sqlite3pp::func::getRow<ActLevel>(*sel_it, 0, sel_qry.column_count());
				string update = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET";
				double bias = Statistics::expected_val(params) / reg_info.max_activity;
				// Remove the last comma
				update += " B_" + reg_info.name + " = " + to_string(bias);
				int rowid = sel_ID.get<int>(0);
				update += " WHERE ROWID=" + to_string(rowid);
				db.execute(update.c_str());

				logging.step();
				sel_it++;
			}
			db.execute("END");
		}
	}
	catch (exception & e) 
	{
		logging.exceptionMessage(e, 3);
	}

	try
	{
		PythonFunctions::configure("select");
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 4);
	}

	return 0;
}