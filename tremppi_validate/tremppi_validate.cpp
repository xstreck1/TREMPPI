#include <tremppi_common/general/logging.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>

#include "io/validate_options.hpp"
#include "io/properties_reader.hpp"
#include "io/parametrization_reader.hpp"
#include "construction/construction_manager.hpp"
#include "construction/product_builder.hpp"
#include "synthesis/synthesis_manager.hpp"

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
	ParametrizationReader par_reader;
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

		// Initiate the parametrizations
		par_reader.select(reg_infos, select, db);
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

	MyKinetics kinetics;
	kinetics.components.emplace_back(MyKinetics::Component{ 0, MyKinetics::Params{}, 1, 1 });
	kinetics.components.emplace_back(MyKinetics::Component{ 1, MyKinetics::Params{}, 1, 1 });
	map < CompID, Levels > requirements = map < CompID, Levels >{};
	kinetics.components[0].params.push_back(MyKinetics::Param{
		"",
		Levels{ 0, 1 },
		move(requirements),
		Levels{ 1 } }
	);
	requirements = map < CompID, Levels >{ { 0, Levels{ 0 } } };
	kinetics.components[1].params.push_back(MyKinetics::Param{
		"A:0",
		Levels{ 0, 1 },
		move(requirements),
		Levels{ 1 } }
	);
	requirements = map < CompID, Levels >{ { 0, Levels{ 1 } } };
	kinetics.components[1].params.push_back(MyKinetics::Param{
		"A:1",
		Levels{ 0, 1 },
		move(requirements),
		Levels{ 2 } }
	);


	ProductStructure product;
	// Construction of data structures
	try {
		product = ConstructionManager::construct(reg_infos, automata[0], kinetics);
	}
	catch (std::exception & e) {
		logging.exceptionMessage(e, 5);
	}

	// Synthesis of parametrizations
	try {
		SynthesisManager synthesis_manager(product);
		size_t BFS_bound = ValidateOptions::getBound(po); ///< Maximal cost on the verified automata[0].

		// Do the computation for all the rounds
		while (par_reader.next()) {
			vector<StateTransition> witness_trans;
			double robustness_val = 0.;
			size_t cost = INF;

			// Call synthesis procedure based on the type of the automata[0].
			switch (product.getMyType()) {
			case BA_finite:
				cost = synthesis_manager.checkFinite(po, witness_trans, robustness_val, par_reader.getRowID());
				break;
			case BA_standard:
				cost = synthesis_manager.checkFull(po, witness_trans, robustness_val, par_reader.getRowID());
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
		logging.exceptionMessage(e, 6);
	}

	return 0;
}
