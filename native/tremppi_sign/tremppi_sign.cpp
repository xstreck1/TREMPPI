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
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/python/python_functions.hpp>

void addColumns(const RegInfos & reg_infos, sqlite3pp::database & db) 
{
	for (const RegInfo & reg_info : reg_infos) 
	{
		for (const auto & regulator : reg_info.regulators) 
		{
			const string reg_name = reg_infos[regulator.first].name;

			for (const ActLevel & threshold : regulator.second) 
			{
				const string column_name = "S_" + reg_name + "_" + to_string(threshold) + "_" + reg_info.name;
				sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, column_name, "STRING", db);
			}
		}
		const string column_name = "E_" + reg_info.name;
		sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, column_name, "INTEGER", db);
	}
	const string column_name = "E_SUM";
	sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, column_name, "INTEGER", db);
}

bool compareContexts(const vector<Levels> & contexts, const vector<vector<size_t> > & context_pairs, const Levels & params, const size_t reg_no, const ActLevel threshold, const bool activating) 
{
	if (threshold == 0) 
	{
		throw runtime_error("Trying to remove an edge that is already absent in effect testing.");
	}

	for (const size_t column_no : cscope(contexts)) 
	{
		if (contexts[column_no][reg_no] == threshold) 
		{
			const size_t remove_column_no = context_pairs[column_no][reg_no];

			if (activating) 
			{
				if (params[remove_column_no] < params[column_no]) 
				{
					return true;
				}
			}

			else 
			{
				if (params[remove_column_no] > params[column_no]) 
				{
					return true;
				}
			}
		}
	}
	return false;
}

inline bool isActivating(const vector<Levels> & contexts, const vector<vector<size_t> > & context_pairs, const Levels & params, const size_t reg_no, const ActLevel threshold) 
{
	return compareContexts(contexts, context_pairs, params, reg_no, threshold, true);
}

inline bool isInhibiting(const vector<Levels> & contexts, const vector<vector<size_t> > & context_pairs, const Levels & params, const size_t reg_no, const ActLevel threshold) 
{
	return compareContexts(contexts, context_pairs, params, reg_no, threshold, false);
}

// Return label for an edge (1 for ambiguous, 0 for inactive)
string getSign(const vector<Levels> & contexts, const vector<vector<size_t> > & context_pairs, const Levels & params, const size_t reg_no, const ActLevel threshold) 
{
	const bool activating = isActivating(contexts, context_pairs, params, reg_no, threshold);
	const bool inhibiting = isInhibiting(contexts, context_pairs, params, reg_no, threshold);

	if (activating & inhibiting) 
	{
		return "1";
	}
	else if (activating & !inhibiting) 
	{
		return "+";
	}
	else if (!activating & inhibiting) 
	{
		return "-";
	}
	else 
	{
		return "0";
	}
}

//
int tremppi_sign(int argc, char ** argv) 
{
	TremppiSystem::initiate("tremppi_sign", argc, argv);
	Logging logging;

	bfs::path database_path = TremppiSystem::DATA_PATH / DATABASE_FILENAME;
	string selection;
	sqlite3pp::database db;
	RegInfos reg_infos;

	try 
	{
		// Get database
		db = move(sqlite3pp::database(database_path.string().c_str()));
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
		addColumns(reg_infos, db);
		logging.newPhase("Expressing component", reg_infos.size());

		for (const RegInfo & reg_info : reg_infos) 
		{
			// For each context (indexed by ordinals, not IDs) hold the context that is obtained by removal of a specific regulator (indexed by ordinals, not IDs)
			vector<vector<size_t> > context_pairs;
			for (const auto & context : reg_info.contexts) 
			{
				vector<size_t> pairs;
				for (const size_t reg_no : cscope(reg_info.regulators)) 
				{
					// The threshold value is 0 => no removal is possible
					if (context.second[reg_no] == 0) 
					{
						pairs.emplace_back(INF);
					}
					else 
					{
						const CompID reg_ID = DataInfo::RegNoToRegID(reg_info, reg_no);
						const size_t column_ID = DataInfo::getColumnWithout(reg_info, context.first, reg_ID, context.second[reg_no]);
						pairs.emplace_back(DataInfo::columnIDToColumnNo(reg_info, column_ID));
					}
				}
				context_pairs.emplace_back(pairs);
			}
			vector<Levels> contexts = DataInfo::contextsVector(reg_info);

			// Select parametrizations and IDs
			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, selection, db);
			sqlite3pp::query sel_IDs = DatabaseReader::selectionIDs(selection, db);
			sqlite3pp::query::iterator sel_it = sel_qry.begin();

			logging.newPhase("Listing parametrizations", DatabaseReader::getSelectionSize(selection,  db));
			db.execute("BEGIN TRANSACTION");
			// Go through parametrizations
			for (auto sel_ID : sel_IDs) 
			{
				Levels params = sqlite3pp::func::getRow<ActLevel>(*sel_it, 0, sel_qry.column_count());
				string update = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET";
				size_t indegree = 0;

				for (const size_t reg_no : cscope(reg_info.regulators)) 
				{
					const CompID reg_ID = DataInfo::RegNoToRegID(reg_info, reg_no);
					for (const ActLevel threshold : reg_info.regulators.at(reg_ID)) 
					{
						const string sign = getSign(contexts, context_pairs, params, reg_no, threshold);
						indegree += sign == "0" ? 0 : 1;
						update += " S_" + reg_infos[reg_ID].name + "_" + to_string(threshold) + "_" + reg_info.name + " = \"" + sign + "\",";
					}
				}
				// Remove the last comma
				update += " E_" + reg_info.name + " = " + to_string(indegree);
				int rowid = sel_ID.get<int>(0);
				update += " WHERE ROWID=" + to_string(rowid);
				db.execute(update.c_str());
				logging.step();
				sel_it++;
			}
			db.execute("END");
		}

		// Count the total edges
		string sum_command = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET E_SUM =";
		for (const RegInfo & reg_info : reg_infos) 
		{
			sum_command += " E_" + reg_info.name + " +";
		}
		sum_command[sum_command.size() - 1] = ' ';
		db.execute(sum_command.c_str());
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