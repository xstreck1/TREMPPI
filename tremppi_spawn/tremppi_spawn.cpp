#include <tremppi_common/general/logging.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/network/constraint_parser.hpp>

#include "compute/parameter_builder.hpp"
#include "compute/parametrizations_builder.hpp"
#include "io/model_reader.hpp"
#include "io/database_filler.hpp"
#include "io/spawn_options.hpp"
#include "io/syntax_checker.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_spawn.
/// - Checks for correctness of a model.
/// - Produces a database of parametrizations based on the model.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_spawn(int argc, char ** argv) {
	bpo::variables_map po = tremppi_system.initiate<SpawnOptions>("tremppi_spawn", argc, argv);
	bfs::path input_path = SpawnOptions::getPath(po, NETWORK_FILENAME);
	bfs::path database_file = input_path / DATABASE_FILENAME;

	// Check the file
	Json::Value root; // root of the network
	try {
		BOOST_LOG_TRIVIAL(info) << "Checking the JSON correctness.";

		root = FileManipulation::readJSasJSON(input_path / NETWORK_FILENAME);

		SyntaxChecker::controlSemantics(root);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	// Parse the model 
	Model model;
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing the network.";

		model = ModelReader::jsonToModel(root);
		for (CompID ID : cscope(model.components))
			ConstraintFomatter::consToFormula(model, ID);
		if (model.components.empty())
			throw runtime_error("No components found in the model.");
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	// Obtain the kinetics
	Kinetics kinetics;
	try {
		BOOST_LOG_TRIVIAL(info) << "Obtaining kinetics.";

		kinetics.components = ParameterBuilder::build(model);
		// First build-check
		ParametrizationsBuilder::build(true, model, kinetics);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}

	// Skip further execution if only conducting a check
	if (po.count("check-only") > 0) {
		BOOST_LOG_TRIVIAL(info) << "Check-only specified, skipping the enumeration.";
		return 0;
	}

	try {
		ParametrizationsBuilder::build(false, model, kinetics);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}

	// Output the data
	try {
		BOOST_LOG_TRIVIAL(info) << "Creating the database file.";
		if (bfs::exists(database_file)) 
			BOOST_LOG_TRIVIAL(warning) << "Database file " << database_file.string() << " already exists, erasing.";
		bfs::remove(database_file);
		
		DatabaseFiller database_filler(model, kinetics, database_file.string());
		database_filler.creteTables();
		database_filler.startOutput();

		logging.newPhase("writing parametrization", KineticsTranslators::getSpaceSize(kinetics));
		for (ParamNo param_no = 0ul; param_no < KineticsTranslators::getSpaceSize(kinetics); param_no++) {
			const string parametrization = KineticsTranslators::createParamString(kinetics, param_no);
			database_filler.addParametrization(parametrization);
			logging.step();
		}

		database_filler.finishOutpout();
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 5);
	}

	BOOST_LOG_TRIVIAL(info) << tremppi_system.PROGRAM_NAME << " finished successfully.";
	return 0;
}
