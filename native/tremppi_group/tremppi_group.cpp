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
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/report/report.hpp>
#include <tremppi_common/python/python_functions.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_group
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ComputedQual
{
	string name;
	map<string, size_t> values;
};

inline void incrementOccurence(const string & value, map<string, size_t> & values)
{
	if (values.count(value) == 0) {
		values.insert(make_pair(value, 1));
	}
	else
	{
		values[value]++;
	}
}

int tremppi_group(int argc, char ** argv)
{
	TremppiSystem::initiate("tremppi_group", argc, argv);
	Logging logging;
	const vector<string> prefixes = { "S", "C" };

	RegInfos reg_infos;
	sqlite3pp::database db;
	DatabaseReader reader;

	vector<string> selections;
	vector<string> sels_name;
	try
	{
		DEBUG_LOG << "Reading data.";
		db = move(sqlite3pp::database((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str()));
		selections = reader.getSelectionList();
		sels_name = reader.getSelectionNames();
		reg_infos = reader.readRegInfos(db);

		if (sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("S_.*"), db).empty())
		{
			throw runtime_error("Sign columns not available in the database.");
		}
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 2);
	}

	// Create a report for each selection
	logging.newPhase("making report", selections.size());
	for (const size_t sel_no : cscope(selections))
	{
		Json::Value out;
		map<size_t, string> columns;
		map<vector<bool>, size_t > counts;

		try
		{
			DEBUG_LOG << "Selection " + sels_name[sel_no];
			out = Report::createSetup(selections[sel_no], sels_name[sel_no]);

			for (const string & prefix : prefixes)
			{
				const auto new_columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex{ prefix + "_.*" }, db);
				columns.insert(WHOLE(new_columns));
			}
		}
		catch (exception & e)
		{
			logging.exceptionMessage(e, 3);
		}

		const size_t row_count = sqlite3pp::func::rowCount(PARAMETRIZATIONS_TABLE, out["setup"]["select"].asString(), db);
		try
		{
			DEBUG_LOG << "Reading the values, computing the statistics.";
			logging.newPhase("Reading row", row_count);
			sqlite3pp::query group_qry = DatabaseReader::selectionFilter(columns, out["setup"]["select"].asString(), db);

			vector<bool> group_data(group_qry.column_count(), false);
			// Read the data
			for (auto row : group_qry)
			{
				for (int i = 0; i < group_qry.column_count(); i++) {

					if (row.column_type(i) == SQLITE_NULL)
					{
						throw runtime_error(string("A null valued entry in the column ") + group_qry.column_name(i));
					}
					bool val;
					if (row.column_type(i) == SQLITE_INTEGER)
					{
						val = row.get<int>(i) != 0;
					}
					else if (row.column_type(i) == SQLITE3_TEXT)
					{
						val = row.get<const char *>(i)[0] != '0';
					}
					group_data[i] = val;
				}

				counts[group_data] += 1;
				logging.step();
			}
		}
		catch (exception & e)
		{
			logging.exceptionMessage(e, 4);
		}

		try
		{
			DEBUG_LOG << "Writing results.";
			// For each graph create the graph data and add configuration details
			for (const pair<vector<bool>, size_t> & result : counts)
			{
				Json::Value result_node;
				result_node["count"] = static_cast<int>(result.second);
				int i = 0;
				for (const pair<size_t, string> column : columns)
				{
					result_node[column.second] = static_cast<int>(result.first[i]);
					i++;
				}
				out["records"].append(result_node);
			}

			FileManipulation::writeJSON(TremppiSystem::DATA_PATH / "group" / (out["setup"]["s_name"].asString() + ".json"), out);
		}
		catch (exception & e)
		{
			logging.exceptionMessage(e, 5);
		}
	}
	try
	{
		PythonFunctions::configure("group");
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 6);
	}

	return 0;
}
