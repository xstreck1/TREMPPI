#include <tremppi_common/general/logging.hpp>

#include "io/program_options.hpp"
#include "auxiliary/output_streamer.hpp"
#include "auxiliary/user_options.hpp"
#include "construction/construction_manager.hpp"
#include "construction/product_builder.hpp"
#include "synthesis/synthesis_manager.hpp"

/**
 * @brief checkDepthBound see if there is not a new BFS depth bound
 */
void checkDepthBound(const bool minimalize_cost, const size_t depth, ParamNo & param_ID, OutputManager & output, size_t & BFS_bound, ParamNo & valid_param_count) {
	if (depth < BFS_bound && minimalize_cost) {
		// Reset the outputs if better was found.
		output_streamer.clear_line(verbose_str);
		param_ID = 0;
		output.eraseData();
		output_streamer.output(verbose_str, "New lowest bound on Cost has been found. Restarting the computation. The current Cost is: " + to_string(depth));
		valid_param_count = 0;
		BFS_bound = depth;
	}
}

using namespace TremppiValidate;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_validate.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_validate(int argc, char ** argv) {
	UserOptions user_options;
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
		OutputManager output(user_options, property, model, kinetics);
		SynthesisManager synthesis_manager(product);
		ParamNo param_count = KineticsTranslators::getSpaceSize(kinetics);
		size_t BFS_bound = user_options.bound_size; ///< Maximal cost on the verified property.
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
				cost = synthesis_manager.checkFinite(witness_trans, robustness_val, param_ID, BFS_bound,
					user_options.compute_wintess, user_options.compute_robustness, property.getMinAcc(), property.getMaxAcc());
				break;
			case BA_standard:
				cost = synthesis_manager.checkFull(witness_trans, robustness_val, param_ID, BFS_bound,
					user_options.compute_wintess, user_options.compute_robustness);
				break;
			default:
				throw runtime_error("Unsupported Buchi automaton type.");
			}

			// Parametrization was considered satisfying.
			if ((cost != INF) ^ (user_options.produce_negative)) {
				checkDepthBound(user_options.minimalize_cost, cost, param_ID, output, BFS_bound, param_count);
				string witness_path = WitnessSearcher::getOutput(user_options.use_long_witnesses, product, witness_trans);
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
