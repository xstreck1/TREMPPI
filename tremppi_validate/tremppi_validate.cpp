#include <tremppi_common/general/logging.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>

#include "io/validate_options.hpp"
#include "io/properties_reader.hpp"
#include "io/parametrization_reader.hpp"
#include "io/output_manager.hpp"
#include "data/construction_manager.hpp"
#include "compute/synthesis_manager.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_validate.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_validate(int argc, char ** argv) {
	bpo::variables_map po = tremppi_system.initiate<ValidateOptions>("tremppi_validate", argc, argv);
	bfs::path input_path;
	string select;
	try {
		input_path = ValidateOptions::getPath(po, DATABASE_FILENAME);
		if (po.count("select") > 0)
			select = ValidateOptions::getFilter(po["select"].as<string>());
		else
			select = "1";
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	RegInfos reg_infos;
	sqlite3pp::database db;
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing database.";

		// Get database
		db = move(sqlite3pp::database((input_path / DATABASE_FILENAME).string().c_str()));

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 1);
	}

	// Check the file
	Json::Value root; // root of the properties
	try {
		BOOST_LOG_TRIVIAL(info) << "Checking the JSON correctness.";

		root = FileManipulation::readJSasJSON(input_path / PROPERTIES_FILENAME);

		PropertiesReader::checkSemantics(root);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	// Parse the properties
	vector<PropertyAutomaton> automata;
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing the properties.";

		automata = PropertiesReader::jsonToProperties(root);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}
	 

	logging.newPhase("Checking properties", automata.size());
	for (const PropertyAutomaton & automaton : automata) {
		ProductStructure product;
		// Construction of data structures
		try {
			BOOST_LOG_TRIVIAL(info) << "Building a product with the automaton: " << automaton.name;
			product = ConstructionManager::construct(reg_infos, automaton);
		}
		catch (std::exception & e) {
			logging.exceptionMessage(e, 5);
		}

		// Synthesis of parametrizations
		ParametrizationReader par_reader;
		par_reader.select(reg_infos, select, db);
		try {
			BOOST_LOG_TRIVIAL(info) << "Validating for an automaton: " << automaton.name;
			
			SynthesisManager synthesis_manager(product);
			OutputManager output(po, reg_infos, automaton.name, db);
			output.outputForm();
			size_t BFS_bound = ValidateOptions::getBound(po); ///< Maximal cost on the verified automaton.

			logging.newSubPhase("Validating parametrizations", sqlite3pp::func::columnCount(PARAMETRIZATIONS_TABLE, select, db));
			// Do the computation for all the rounds
			sqlite3pp::transaction xct(db);
			while (par_reader.next()) {
				tuple<size_t, double, vector<StateTransition> > result;

				// Call synthesis procedure based on the type of the automaton.
				switch (product.getMyType()) {
				case BA_finite:
					result = synthesis_manager.checkFinite(po, par_reader.getParametrization());
					break;
					/*case BA_standard:
						cost = synthesis_manager.checkFull(po, par_reader.getParametrization(), witness_trans, robustness_val);
						break;*/
				default:
					throw runtime_error("Unsupported Buchi automaton type.");
				}

				// Parametrization was considered satisfying.
				string witness_path = WitnessSearcher::getOutput(ValidateOptions::getTracteType(po), product, get<2>(result));

				output.outputRound(get<0>(result), get<1>(result), witness_path, par_reader.getParametrization(), par_reader.getRowID());
				logging.subStep();
			}
			xct.commit();
			logging.step();
		}
		catch (std::exception & e) {
			logging.exceptionMessage(e, 6);
		}
	}

	return 0;
}
