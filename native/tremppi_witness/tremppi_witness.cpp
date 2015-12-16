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

#include "io/witness_reader.hpp"
#include "io/witness_output.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_witness.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_witness(int argc, char ** argv)
{
	TremppiSystem::initiate("tremppi_witness", argc, argv);
	Logging logging;

	RegInfos reg_infos;
	sqlite3pp::database db;
	vector<string> selections;
	vector<string> sels_name;
	DatabaseReader reader;
	try
	{
		DEBUG_LOG << "Reading data.";
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
		Json::Value properties;
		ParamNo selection_size;;
		// Obtain data
		try
		{
			DEBUG_LOG << "Selection " + sels_name[sel_no];
			out = Report::createSetup(selections[sel_no], sels_name[sel_no]);

			// Get properties 
			properties = FileManipulation::parseJSON(TremppiSystem::DATA_PATH / PROPERTIES_FILENAME);

			// Read regulatory information
			selection_size = DatabaseReader::getSelectionSize(out["setup"]["select"].asString(), db);
		}
		catch (exception & e)
		{
			logging.exceptionMessage(e, 3);
		}

		// Obtain the nodes and write to the the set
		map<pair<string, string>, size_t> transitions;
		WitnessReader wit_reader;
		size_t properties_count = size_t{};
		try
		{
			DEBUG_LOG << "Reading data.";
			logging.newPhase("computing witness", properties["records"].size());
			for (const auto & property : properties["records"])
			{
				// If selected
				if (property["select"].asBool())
				{
					properties_count++;
					const string name = property["name"].asString();
					out["setup"]["properties"] = out["setup"]["properties"].asString() + name + ",";

					wit_reader.select(name, out["setup"]["select"].asString(), db);
					// Read transitions
					logging.newPhase("Listing parametrizations", selection_size);
					while (wit_reader.next())
					{
						map<pair<string, string>, size_t> new_transitions = wit_reader.getWitness();
						for (const pair<pair<string, string>, size_t> & transition : new_transitions) {
							transitions[transition.first] += transition.second;
						}
						logging.step();
					}
				}
			}
		}
		catch (exception & e)
		{
			logging.exceptionMessage(e, 4);
		}
		try
		{
			DEBUG_LOG << "Writing data.";

			out["elements"] = WitnessOutput::convert(transitions, selection_size * properties_count);

			FileManipulation::writeJSON(TremppiSystem::DATA_PATH / "witness" / (out["setup"]["s_name"].asString() + ".json"), out);
		}
		catch (exception & e)
		{
			logging.exceptionMessage(e, 5);
		}
	}

	try
	{
		PythonFunctions::configure("witness");
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 6);
	}

	return 0;
}
