#include <tremppi_common/general/logging.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>

#include "io/validate_options.hpp"
#include "io/properties_reader.hpp"
#include "construction/construction_manager.hpp"
#include "construction/product_builder.hpp"
#include "synthesis/synthesis_manager.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_validate.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_validate(int argc, char ** argv) {
	bpo::variables_map po = tremppi_system.initiate<ValidateOptions>("tremppi_validate", argc, argv);
	bfs::path input_path = ValidateOptions::getPath(po, DATABASE_FILENAME);

	RegInfos reg_infos;
	sqlite3pp::database db;
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing database.";

		// Get database
		db = move(sqlite3pp::database((input_path / DATABASE_FILENAME).string().c_str()));

		// Read regulatory information
		sqlite3pp::query qry(db, ("SELECT " + DatabaseReader::NAMES_COLUMN + " FROM " + COMPONENTS_TABLE).c_str());
		for (auto row : qry) {
			string name = row.get<const char*>(0);
			reg_infos.push_back(DatabaseReader::readRegInfo(reg_infos.size(), name, db));
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	// Check the file
	Json::Value root; // root of the network
	try {
		BOOST_LOG_TRIVIAL(info) << "Checking the JSON correctness.";

		root = FileManipulation::readJSasJSON(input_path / PROPERTIES_FILENAME);

		PropertiesReader::checkSemantics(root);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	vector<PropertyAutomaton> automata(1);
	Kinetics kinetics;
	ProductStructure product;

	automata[0] = PropertyAutomaton();
	automata[0].addState("ser0", false);
	automata[0].addState("ser1", false);
	automata[0].addState("ser2", true);
	automata[0].addEdge(0, 1, { "(A=0|B=0)" });
	automata[0].addEdge(1, 1, { "tt" });
	automata[0].addEdge(1, 2, { "(A=1|B=2)" });
	automata[0].addEdge(2, 2, { "ff" });
	kinetics.components.emplace_back(Kinetics::Component{ 0, Kinetics::Params{}, 1, 1 });
	kinetics.components.emplace_back(Kinetics::Component{ 1, Kinetics::Params{}, 1, 1 });
	map < CompID, Levels > requirements = map < CompID, Levels >{};
	kinetics.components[0].params.push_back(Kinetics::Param{
		"",
		Levels{ 0, 1 },
		move(requirements),
		Levels{ 1 } }
	);
	requirements = map < CompID, Levels >{ { 0, Levels{ 0 } } };
	kinetics.components[1].params.push_back(Kinetics::Param{
		"A:0",
		Levels{ 0, 1 },
		move(requirements),
		Levels{ 1 } }
	);
	requirements = map < CompID, Levels >{ { 0, Levels{ 1 } } };
	kinetics.components[1].params.push_back(Kinetics::Param{
		"A:1",
		Levels{ 0, 1 },
		move(requirements),
		Levels{ 2 } }
	);

	// Construction of data structures
	try {
		product = ConstructionManager::construct(reg_infos, automata[0], kinetics);
	}
	catch (std::exception & e) {
		logging.exceptionMessage(e, 3);
	}

	// Synthesis of parametrizations
	try {
		SynthesisManager synthesis_manager(product);
		ParamNo param_count = KineticsTranslators::getSpaceSize(kinetics);
		size_t BFS_bound = ValidateOptions::getBound(po); ///< Maximal cost on the verified automata[0].

		// Do the computation for all the rounds
		for (ParamNo param_ID = 0; param_ID < param_count; param_ID++) {
			vector<StateTransition> witness_trans;
			double robustness_val = 0.;
			size_t cost = INF;

			// Call synthesis procedure based on the type of the automata[0].
			switch (product.getMyType()) {
			case BA_finite:
				cost = synthesis_manager.checkFinite(po, witness_trans, robustness_val, param_ID);
				break;
			case BA_standard:
				cost = synthesis_manager.checkFull(po, witness_trans, robustness_val, param_ID);
				break;
			default:
				throw runtime_error("Unsupported Buchi automaton type.");
			}

			// Parametrization was considered satisfying.
			if ((cost != INF)) {
				string witness_path = WitnessSearcher::getOutput(ValidateOptions::getTracteType(po), product, witness_trans);
			}
		}
	}
	catch (std::exception & e) {
		logging.exceptionMessage(e, 4);
	}

	return 0;
}
