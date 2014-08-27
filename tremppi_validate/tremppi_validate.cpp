#include <tremppi_common/general/logging.hpp>

#include "io/program_options.hpp"
#include "auxiliary/output_streamer.hpp"
#include "construction/construction_manager.hpp"
#include "construction/product_builder.hpp"
#include "synthesis/synthesis_manager.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_validate.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_validate(int argc, char ** argv) {
	bpo::variables_map po; // program options provided on the command line
	bfs::path input_path; // an input path
	try {
		if (argc < 1)
			throw runtime_error("No parameters.");

		po = ValidateOptions::parseProgramOptions(argc, argv);
		input_path = ValidateOptions::getDatabasePath(po);

		tremppi_system.set("tremppi_validate", argv[0], input_path.parent_path());
		logging.init(1);
		BOOST_LOG_TRIVIAL(info) << "TREMPPI validation by LTL model checking (" << tremppi_system.PROGRAM_NAME << ") started.";
	}
	catch (exception & e) {
		cerr << e.what();
		return 1;
	}

	Model model;
	PropertyAutomaton property;
	Kinetics kinetics;
	ProductStructure product;

	model.components.emplace_back(Model::ModelComp{ 0, "A", 1, Model::Regulations{}, vector < string > {} });
	model.components.emplace_back(Model::ModelComp{ 0, "B", 3, Model::Regulations{}, vector < string > {} });
	model.components[1].regulations.emplace_back(Model::Regulation{ 0, 1, "" });
	property = PropertyAutomaton();
	property.addState("ser0", false);
	property.addState("ser1", false);
	property.addState("ser2", true);
	property.addEdge(0, 1, { "(A=0|B=0)" });
	property.addEdge(1, 1, { "tt" });
	property.addEdge(1, 2, { "(A=1|B=3)" });
	property.addEdge(2, 2, { "ff" });
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
		Levels{ 3 } }
	);

	// Construction of data structures
	try {
		product = ConstructionManager::construct(model, property, kinetics);
	}
	catch (std::exception & e) {
		output_streamer.output(error_str, string("Error occured while building the data structures: \"" + string(e.what()) + "\". \n Contact support for details."));
		return 4;
	}

	// Synthesis of parametrizations
	try {
		OutputManager output(po, property, model, kinetics);
		SynthesisManager synthesis_manager(product);
		ParamNo param_count = KineticsTranslators::getSpaceSize(kinetics);
		size_t BFS_bound = ValidateOptions::getBound(po); ///< Maximal cost on the verified property.
		output.outputForm();

		// Do the computation for all the rounds
		for (ParamNo param_ID = 0; param_ID < param_count; param_ID++) {
			output.outputRoundNo(param_ID + 1, param_count);

			vector<StateTransition> witness_trans;
			double robustness_val = 0.;
			size_t cost = INF;

			// Call synthesis procedure based on the type of the property.
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
				output.outputRound(param_ID, cost, robustness_val, witness_path);
				param_count++;
			}
		}
	}
	catch (std::exception & e) {
		output_streamer.output(error_str, string("Error occured while syntetizing the parametrizations: \"" + string(e.what()) + "\".\n Contact support for details."));
		return 5;
	}

	return 0;
}
