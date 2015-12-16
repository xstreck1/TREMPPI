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
#include <tremppi_common/report/report.hpp>
#include <tremppi_common/python/python_functions.hpp>

#include "compute/compute.hpp"
#include "io/output.hpp"

int tremppi_correlations(int argc, char ** argv) 
{
	TremppiSystem::initiate("tremppi_correlations", argc, argv);
	Logging logging;

	RegInfos reg_infos;
	sqlite3pp::database db;
	vector<string> selections;
	vector<string> sels_name;
	DatabaseReader reader;
	try {
		DEBUG_LOG << "Parsing data file.";
		db = move(sqlite3pp::database((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str()));
		selections = reader.getSelectionList();
		sels_name = reader.getSelectionNames();
		reg_infos = reader.readRegInfos(db);
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
		vector<sqlite3pp::query> queries;

		try
		{
			DEBUG_LOG << "Selection " + sels_name[sel_no];
			out = Report::createSetup(selections[sel_no], sels_name[sel_no]);

			for (const RegInfo & reg_info : reg_infos)
			{
				map<size_t, string> columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("B_" + reg_info.name), db);
				if (columns.empty())
				{
					throw runtime_error("did not find the column B_" + reg_info.name);
				}
				queries.emplace_back(DatabaseReader::selectionFilter(columns, out["setup"]["select"].asString(), db));
			}
		}
		catch (exception & e)
		{
			logging.exceptionMessage(e, 3);
		}

		FunsData funs_data;
		try
		{
			DEBUG_LOG << "Computing function graph data.";
			Compute::deviation(reg_infos, out["setup"]["size"].asInt(), queries, logging, funs_data);
			Compute::correlation(reg_infos, out["setup"]["size"].asInt(), queries, logging, funs_data);
		}
		catch (exception & e)
		{
			logging.exceptionMessage(e, 4);
		}
		try
		{
			DEBUG_LOG << "Writing output.";
			out["elements"] = Output::functionalData(funs_data);
			FileManipulation::writeJSON(TremppiSystem::DATA_PATH / "correlations" / (out["setup"]["s_name"].asString() + ".json"), out);
		}
		catch (exception & e)
		{
			logging.exceptionMessage(e, 5);
		}

	}

	try
	{
		PythonFunctions::configure("correlations");
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 6);
	}
	return 0;
}