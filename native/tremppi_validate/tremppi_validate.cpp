#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/python/python_functions.hpp>

#include "io/properties_reader.hpp"
#include "io/parametrization_reader.hpp"
#include "io/output_manager.hpp"
#include "data/construction_manager.hpp"
#include "compute/analysis_manager.hpp"

struct ValidateSetup 
{
	bool cost;
	bool trace;
	bool robustness;
} validate_setup;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_validate.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_validate() 
{
	Logging logging;
	string select;

	RegInfos reg_infos;
	sqlite3pp::database db;
	try
	{
		DEBUG_LOG << "Parsing database.";

		// Get selection		
		select = DatabaseReader::getSelectionTerm();

		// Get database
		db = move(sqlite3pp::database((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str()));

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 2);
	}

	// Check the file
	Json::Value properties; // root of the properties
	try
	{
		DEBUG_LOG << "Checking the JSON correctness.";

		Json::Value root = FileManipulation::parseJSON(TremppiSystem::DATA_PATH / PROPERTIES_FILENAME);
		properties = root["records"];
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 3);
	}

	// Parse the properties
	vector<PropertyInfo> automata;
	try
	{
		DEBUG_LOG << "Parsing the properties.";
		automata = PropertiesReader::jsonToProperties(properties);
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 4);
	}

	// Conduct the check for each of the properties
	logging.newPhase("Checking properties", automata.size());
	for (const PropertyInfo & automaton : automata)
	{
		ProductStructure product;
		// Construction of data structures
		try
		{
			DEBUG_LOG << "Building a product with the automaton: " << automaton.name;
			ConstructionManager::construct(reg_infos, automaton, product);
		}
		catch (std::exception & e)
		{
			logging.exceptionMessage(e, 5);
		}

		// Analysis of parametrizations
		ParametrizationReader par_reader;
		par_reader.select(reg_infos, select, db);
		try
		{
			DEBUG_LOG << "Validating for an automaton: " << automaton.name;

			AnalysisManager analysis_manager(product, automaton.bound, validate_setup.trace, validate_setup.robustness);
			OutputManager output(validate_setup.cost, validate_setup.trace, validate_setup.robustness, reg_infos, automaton.name, db);
			output.outputForm();

			logging.newPhase("Validating parametrizations", sqlite3pp::func::rowCount(PARAMETRIZATIONS_TABLE, select, db));
			// Do the computation for all the rounds
			sqlite3pp::transaction xct(db);
			while (par_reader.next())
			{
				tuple<size_t, multimap<StateID, StateID>, double> result;

				result = analysis_manager.check(par_reader.getParametrization());

				// Parametrization was considered satisfying.
				string witness_path;
				if (get<0>(result) != INF && validate_setup.trace)
				{
					witness_path = WitnessSearcher::getOutput(product, get<0>(result), get<1>(result));
				}

				output.outputRound(get<0>(result), get<2>(result), witness_path, par_reader.getParametrization(), par_reader.getRowID());
				logging.step();
			}
			xct.commit();
		}
		catch (std::exception & e)
		{
			logging.exceptionMessage(e, 6);
		}
	}

	try
	{
		PythonFunctions::configure("select");
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 7);
	}

	return 0;
}

int tremppi_cost(int argc, char ** argv) {
	validate_setup = ValidateSetup{ true, false, false };
	TremppiSystem::initiate("tremppi_cost", argc, argv);
	return tremppi_validate();
}

int tremppi_trace(int argc, char ** argv) {
	validate_setup = ValidateSetup{ false, true, false };
	TremppiSystem::initiate("tremppi_trace", argc, argv);
	return tremppi_validate();
}

int tremppi_robustness(int argc, char ** argv) {
	validate_setup = ValidateSetup{ false, false, true };
	TremppiSystem::initiate("tremppi_robustness", argc, argv);
	return tremppi_validate();
}
