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

	Json::Value out;
	Json::Value properties;
	RegInfos reg_infos;
	sqlite3pp::database db;
	ParamNo selection_size;;
	// Obtain data
	try 
	{
		DEBUG_LOG << "Parsing data.";

		// Create setup
		out = Report::createSetup();

		// Get properties 
		properties = FileManipulation::parseJSON(TremppiSystem::DATA_PATH / PROPERTIES_FILENAME);

		// Get database
		db = move(sqlite3pp::database((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str()));
		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
		selection_size = DatabaseReader::getSelectionSize(out["setup"]["select"].asString(), db);
	}
	catch (exception & e) 
	{
		logging.exceptionMessage(e, 2);
	}

	// Obtain the nodes and write to the the set
	map<pair<string, string>, size_t> transitions;
	WitnessReader wit_reader;
	size_t properties_count = size_t{};
	try 
	{
		DEBUG_LOG << "Loading witnesses.";
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
		logging.exceptionMessage(e, 3);
	}
	try 
	{
		DEBUG_LOG << "Converting to JSON";

		out["elements"] = WitnessOutput::convert(transitions, selection_size * properties_count);
	}
	catch (exception & e) 
	{
		logging.exceptionMessage(e, 4);
	}

	// Output 
	try 
	{
		DEBUG_LOG << "Writing output.";
		FileManipulation::writeJSON(TremppiSystem::DATA_PATH / "witness" / (out["setup"]["s_name"].asString() + ".json"), out);
	}
	catch (exception & e) 
	{
		logging.exceptionMessage(e, 5);
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
