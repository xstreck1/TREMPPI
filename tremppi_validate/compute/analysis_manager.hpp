#pragma once

#include "../data/property_automaton.hpp"

#include "witness_searcher.hpp"
#include "visit_storage.hpp"
#include "model_checker.hpp"
#include "robustness_compute.hpp"
#include "checker_setting.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief STEP 3 - Control class for the computation.
///
/// Manager of the analysis procedure - takes the reference data constructed during previous steps and computes and executes the analysis.
/// Analysis is done in three steps:
///	-# preparation: empties data and starts a new round.
///   -# analysis: computes the coloring, stored in the storage object and adds data to coloring analyzer if needed.
///   -# conclusion: stores additional data and outputs
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AnalysisManager {
	const ProductStructure & product;
	unique_ptr<ModelChecker> model_checker; ///< Class for analysis.
	unique_ptr<VisitStorage> storage; ///< Class that holds.
	unique_ptr<WitnessSearcher> searcher; ///< Class to build wintesses.
	unique_ptr<RobustnessCompute> computer; ///< Class to compute robustness.

public:
	/**
	 * Constructor builds all the data objects that are used within.
	 */
	AnalysisManager(const ProductStructure & _product) : product(_product) {
		storage.reset(new VisitStorage(product.getStateCount()));
		model_checker.reset(new ModelChecker(product));
		searcher.reset(new WitnessSearcher(product));
		computer.reset(new RobustnessCompute(product));
	}

	/**
	 * @brief checkFull conduct model check for a lasso
	 */
	tuple<size_t, multimap<StateID, StateID>, double >  checkFull(const size_t bfs_bound, const TraceType trace_type, const Levels & parametrization) {
		tuple<size_t, multimap<StateID, StateID>, double > result;
		get<2>(result) = 0.0;

		// Find the minimal cost - step 1
		get<0>(result) = INF;
		CheckerSetting settings;
		settings.bfs_bound = bfs_bound - 1; // We need at least one step for the loop on the final state
		settings.bound_type = BoundType::step;
		settings.circ = false;
		VisitStorage reach_storage = model_checker->conductCheck(settings, parametrization);
		// step 2
		settings.bound_type = BoundType::min;
		settings.circ = true;
		for (const StateID ID : product.getFinalStates()) {
			if (reach_storage.isFound(ID)) {
				settings.initial_states = settings.final_states = { ID };
				settings.bfs_bound = bfs_bound - reach_storage.getVisit(ID);
				VisitStorage loop_storage = model_checker->conductCheck(settings, parametrization);
				get<0>(result) = std::min(reach_storage.getVisit(ID) + loop_storage.getCost(), get<0>(result));
			}
		}

		// Compute the analysis for the lasso
		if (get<0>(result) != INF && trace_type != TraceType::none) {
			for (const StateID ID : product.getFinalStates()) {
				if (reach_storage.isFound(ID)) {
					// Get the loop visits
					settings.initial_states = settings.final_states = { ID };
					settings.bfs_bound = bfs_bound - reach_storage.getVisit(ID);
					settings.circ = true;
					VisitStorage loop_storage = model_checker->conductCheck(settings, parametrization);

					if (reach_storage.getVisit(ID) + loop_storage.getCost() <= get<0>(result)) {
						// Compute reach analysis
						settings.initial_states.clear(); // Reset initals to all
						settings.circ = false;
						auto reach_wit = searcher->findWitnesses(settings, parametrization, reach_storage);
						double reach_rob = computer->compute(settings, parametrization, reach_storage, reach_wit);

						// Compute loop analysis
						settings.initial_states = { ID };
						settings.circ = true;
						auto loop_wit = searcher->findWitnesses(settings, parametrization, loop_storage);
						double loop_rob = computer->compute(settings, parametrization, loop_storage, loop_wit) - 1;

						// Merge
						if (trace_type == TraceType::wit)
							merge(WHOLE(reach_wit), WHOLE(loop_wit), inserter(get<1>(result), end(get<1>(result))));
						get<2>(result) += reach_rob * loop_rob;
					}
				}
			}
		}

		return result;
	}

	/**
	 * @brief checkFinite conduct model check with reachability only
	 */
	tuple<size_t, multimap<StateID, StateID>, double > checkFinite(const size_t bfs_bound, const TraceType trace_type, const Levels & parametrization) {
		tuple<size_t, multimap<StateID, StateID>, double > result;

		CheckerSetting settings;
		settings.bfs_bound = bfs_bound;
		settings.bound_type = BoundType::min;
		settings.circ = false;
		VisitStorage storage = model_checker->conductCheck(settings, parametrization);

		if (storage.succeeded() && trace_type != TraceType::none) {
			auto transitions = searcher->findWitnesses(settings, parametrization, storage);
			if (trace_type == TraceType::wit)
				get<1>(result) = transitions;

			get<2>(result) = computer->compute(settings, parametrization, storage, transitions);
		}

		get<0>(result) = storage.getCost();
		return result;
	}
};