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

#include <tremppi_common/general/system.hpp>
#include <tremppi_common/python/python_functions.hpp>

#include "compute/MVQMC.hpp"
#include "io/output.hpp"

//
int tremppi_express(int argc, char ** argv) 
{
	TremppiSystem::initiate("tremppi_express", argc, argv);
	Logging logging;

	bfs::path database_path = TremppiSystem::DATA_PATH / DATABASE_FILENAME;
	string select;
	map<string, ActLevel> maxes;
	RegFuncs functions;
	sqlite3pp::database db;

	try 
	{
		// Get database
		db = move(sqlite3pp::database(database_path.string().c_str() ));

		// Get selection (use empty on server to prevent errors by insufficient data on the server)
		select = TremppiSystem::called_from_server ? "" : DatabaseReader::getSelectionTerm();

		auto res = DatabaseReader::getSelectionList();

		DatabaseReader reader;
		RegInfos infos = reader.readRegInfos(db);

		// Obtain the components data

		for (auto & info : infos) 
		{
			Configurations minterms;
			for (const pair<size_t, string> column : info.columns)
				minterms.emplace_back(DataConv::getThrsFromContext(column.second));
			functions.emplace_back(RegFunc{ move(info), move(minterms) });
		}
	}
	catch (exception & e) 
	{
		logging.exceptionMessage(e, 2);
	}

	// Convert and output
	try 
	{
		Output::addColumns(functions, db);
		logging.newPhase("Expressing component", functions.size());
		for (const RegFunc & reg_func : functions) 
		{
			// Select parametrizations and IDs
			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_func.info.columns, select, db);
			sqlite3pp::query sel_IDs = DatabaseReader::selectionIDs(select, db);
			sqlite3pp::query::iterator sel_it = sel_qry.begin();

			logging.newPhase("Listing parametrizations", DatabaseReader::getSelectionSize(select, db));
			db.execute("BEGIN TRANSACTION");
			// Go through parametrizations
            for (auto sel_ID : sel_IDs) 
            {
				vector<vector<PMin>> config_values(reg_func.info.max_activity + 1);
				Levels params = sqlite3pp::func::getRow<ActLevel>(*sel_it, 0, sel_qry.column_count());
				
				// Convert the contexts 
				for (const size_t context_id : cscope(params)) 
				{
					PMin minterm(reg_func.minterms[context_id].size());
					transform(WHOLE(reg_func.minterms[context_id]), begin(minterm), [](ActLevel act_level){return PLit{ act_level }; });
					config_values[params[context_id]].emplace_back(minterm);
				}

				// Minimize for all values
				vector<PDNF> pdnfs(1);
				for (ActLevel target_val = 1; target_val < (reg_func.info.max_activity + 1); target_val++) 
				{
					pdnfs.emplace_back(MVQMC::compactize(config_values[target_val]));
				}
				
				string formula = "\"" + Output::pdnfToFormula(functions, reg_func, pdnfs) + "\"";
				int rowid = sel_ID.get<int>(0);
				string update = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET F_" + reg_func.info.name + "=" + formula + " WHERE ROWID=" + to_string(rowid);
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
