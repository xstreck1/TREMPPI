#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/network/constraint_parser.hpp>
#include <tremppi_common/python/python_functions.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include "compute/parameter_builder.hpp"
#include "compute/parametrizations_builder.hpp"
#include "io/model_reader.hpp"
#include "io/database_filler.hpp"
#include "io/syntax_checker.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_spawn.
/// - Checks for correctness of a model.
/// - Produces a database of parametrizations based on the model.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_spawn(int argc, char ** argv)
{
	TremppiSystem::initiate("tremppi_spawn", argc, argv);
	Logging logging;
	bfs::path database_file = TremppiSystem::DATA_PATH / DATABASE_FILENAME;

	// Check the file
	Json::Value root; // root of the network
	try
	{
		DEBUG_LOG << "Checking the JSON correctness.";

		root = FileManipulation::parseJSON(TremppiSystem::DATA_PATH / NETWORK_FILENAME);

		SyntaxChecker::controlSemantics(root);
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 2);
	}

	// Parse the model 
	Model model;
	try
	{
		DEBUG_LOG << "Parsing the network.";

		model = ModelReader::jsonToModel(root);
		for (CompID ID : cscope(model.components))
			ConstraintFomatter::consToFormula(model, ID);
		if (model.components.empty())
			throw runtime_error("No components found in the model.");
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 3);
	}

	// Obtain the kinetics
	Kinetics kinetics;
	try
	{
		DEBUG_LOG << "Obtaining kinetics.";

		kinetics.components = ParameterBuilder::build(model);
		// First build-check
		ParametrizationsBuilder::build(true, model, kinetics);
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 4);
	}

	// Build parametrizations (in full this time)
	try
	{
		ParametrizationsBuilder::build(false, model, kinetics);
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 5);
	}

	// Output the database
	try
	{
		DEBUG_LOG << "Creating the database file.";
		if (bfs::exists(database_file))
		{
			WARNING_LOG << "Database file " << database_file.string() << " already exists, erasing.";
		}
		bfs::remove(database_file);

		DatabaseFiller database_filler(model, kinetics, database_file.string());
		database_filler.creteTables();
		database_filler.startOutput();

		DEBUG_LOG << "Writing the database file, in total " + to_string(KineticsTranslators::getSpaceSize(kinetics)) + " parametrizations.";
		logging.newPhase("writing parametrization", KineticsTranslators::getSpaceSize(kinetics));
		for (ParamNo param_no = 0ul; param_no < KineticsTranslators::getSpaceSize(kinetics); param_no++)
		{
			const string parametrization = KineticsTranslators::createParamString(kinetics, param_no);
			database_filler.addParametrization(parametrization);
			logging.step();
		}

		database_filler.finishOutpout();
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 6);
	}

	try
	{
		PythonFunctions::configure("select");
		PythonFunctions::configure("properties");
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 7);
	}

	return 0;
}
