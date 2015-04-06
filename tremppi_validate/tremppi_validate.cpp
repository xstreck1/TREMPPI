#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>

#include "io/validate_options.hpp"
#include "io/properties_reader.hpp"
#include "io/parametrization_reader.hpp"
#include "io/output_manager.hpp"
#include "data/construction_manager.hpp"
#include "compute/analysis_manager.hpp"

// TODO: Check if the transition generation is correct for all three components (w.r.t. the transition system used)
// TODO: Components still required to be ordered...

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_validate.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_validate(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<ValidateOptions>("tremppi_validate", argc, argv);
	Logging logging;
	string select;

	RegInfos reg_infos;
	sqlite3pp::database db;
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing database.";

		// Get selection		
		select = DatabaseReader::getSelectionTerm();

		// Get database
		db = move(sqlite3pp::database((TremppiSystem::WORK_PATH / DATABASE_FILENAME).string().c_str()));

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	// Check the file
	Json::Value properties; // root of the properties
	try {
		BOOST_LOG_TRIVIAL(info) << "Checking the JSON correctness.";

		Json::Value root = FileManipulation::parseJSON(TremppiSystem::WORK_PATH / PROPERTIES_FILENAME);
		properties = root["list"]["records"];
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	// Parse the properties
	vector<PropertyAutomaton> automata;
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing the properties.";

		automata = PropertiesReader::jsonToProperties(reg_infos, properties);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}


	// Conduct the check for each of the properties
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

		// Analysis of parametrizations
		ParametrizationReader par_reader;
		par_reader.select(reg_infos, select, db);
		try {
			BOOST_LOG_TRIVIAL(info) << "Validating for an automaton: " << automaton.name;

			AnalysisManager analysis_manager(product);
			OutputManager output(automaton.witness, automaton.robustness, reg_infos, automaton.name, db);
			output.outputForm();

			logging.newPhase("Validating parametrizations", sqlite3pp::func::rowCount(PARAMETRIZATIONS_TABLE, select, db));
			// Do the computation for all the rounds
			sqlite3pp::transaction xct(db);
			while (par_reader.next()) {
				tuple<size_t, multimap<StateID, StateID>, double> result;

				// Call analysis procedure based on the type of the automaton.
				switch (product.getMyType()) {
				case BA_finite:
					result = analysis_manager.checkFinite(automaton.bound, automaton.witness, automaton.robustness, par_reader.getParametrization());
					break;
				case BA_standard:
					result = analysis_manager.checkFull(automaton.bound, automaton.witness, automaton.robustness, par_reader.getParametrization());
					break;
				default:
					throw runtime_error("Unsupported Buchi automaton type.");
				}

				// Parametrization was considered satisfying.
				string witness_path;
				if (get<0>(result) != INF && automaton.witness)
					witness_path = WitnessSearcher::getOutput(product, get<0>(result), get<1>(result));

				output.outputRound(get<0>(result), get<2>(result), witness_path, par_reader.getParametrization(), par_reader.getRowID());
				logging.step();
			}
			xct.commit();
			DatabaseReader::makeSelect();
		}
		catch (std::exception & e) {
			logging.exceptionMessage(e, 6);
		}
	}

	return 0;
}
