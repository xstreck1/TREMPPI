#pragma once

#include "../data/property_automaton.hpp"

#include "witness_searcher.hpp"
#include "color_storage.hpp"
#include "model_checker.hpp"
#include "robustness_compute.hpp"
#include "checker_setting.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief STEP 3 - Control class for the computation.
///
/// Manager of the synthesis procedure - takes the reference data constructed during previous steps and computes and executes the synthesis.
/// Synthesis is done in three steps:
///	-# preparation: empties data and starts a new round.
///   -# synthesis: computes the coloring, stored in the storage object and adds data to coloring analyzer if needed.
///   -# conclusion: stores additional data and outputs
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SynthesisManager {
	unique_ptr<ModelChecker> model_checker; ///< Class for synthesis.
	unique_ptr<ColorStorage> storage; ///< Class that holds.
	unique_ptr<WitnessSearcher> searcher; ///< Class to build wintesses.
	unique_ptr<RobustnessCompute> computer; ///< Class to compute robustness.

	/**
	 * @brief analyseLasso Parametrization is know to be satisfiable, make analysis of it.
	 */
	/*void analyseLasso(const pair<StateID, size_t> & final, vector<StateTransition> & trans, const ParamNo param_no, double & robust) {
		SynthesisResults results;
		CheckerSettings settings;
		// First find the coloring from the initial states to the given final.
		settings.final_states = { final.first };
		settings.bound_type = BoundType::min;
		settings.mark_initals = true;
		settings.param_no = param_no;
		results = model_checker->conductCheck(settings);
		searcher->findWitnesses(results, settings);
		trans = searcher->getTransitions();
		computer->compute(results, searcher->getTransitions(), settings);
		robust = computer->getRobustness();

		// Second find a cycle on the final state.
		settings.mark_initals = false;
		settings.initial_states = { final.first };
		results = model_checker->conductCheck(settings);
		searcher->findWitnesses(results, settings);
		const vector<StateTransition> & trans_ref = searcher->getTransitions();
		trans.insert(trans.begin(), trans_ref.begin(), trans_ref.end());
		computer->compute(results, searcher->getTransitions(), settings);
		robust *= computer->getRobustness();
	}*/

	/**
	 * @brief computeLasso parametrization is know to reach a final state, test that state for a bounded loop.
	 */
	/*size_t computeLasso(const bpo::variables_map & po, const pair<StateID, size_t> & final, vector<StateTransition> & trans, const ParamNo param_no, double & robust) {
		CheckerSettings settings;
		settings.bound_type = BoundType::min;
		settings.param_no = param_no;
		settings.initial_states = settings.final_states = { final.first };
		const size_t BFS_bound = ValidateOptions::getBound(po);
		settings.bfs_bound = BFS_bound == INF ? BFS_bound : (BFS_bound - final.second);

		SynthesisResults results = model_checker->conductCheck(settings);
		const size_t cost = results.getLowerBound() == INF ? INF : results.getLowerBound() + final.second;
		if (results.isAccepting() && ValidateOptions::getTracteType(po) != TraceType::none)
			analyseLasso(final, trans, param_no, robust);

		return cost;
	}*/

public:
	SynthesisManager() {}

	/**
	 * Constructor builds all the data objects that are used within.
	 */
	SynthesisManager(const ProductStructure & product)  {
		storage.reset(new ColorStorage(product));
		model_checker.reset(new ModelChecker(product, *storage));
		searcher.reset(new WitnessSearcher(product, *storage));
		computer.reset(new RobustnessCompute(product, *storage));
	}

	/**
	 * @brief checkFull conduct model check with only reachability
	 * @param[in] witnesses for all the shortest cycles
	 * @param[in] robustness_val  robustness of the whole computation
	 * @param param_no number of parametrization to test
	 * @param BFS_bound current bound on depth
	 * @param witnesses should compute witnesses
	 * @param robustness should compute robustness
	 * @return the Cost value for this parametrization
	 */
	/*size_t checkFull(const bpo::variables_map & po, const Levels & parametrization, vector<StateTransition> & trans, double & robustness_val) {
	   CheckerSettings settings;
	   settings.bfs_bound = ValidateOptions::getBound(po);
	   settings.bound_type = ValidateOptions::getBoundType(po);
	   settings.param_no = param_no;
	   settings.mark_initals = true;
	   SynthesisResults results = model_checker->conductCheck(settings);

	   size_t cost = INF;
	   map<StateID, size_t> finals = results.found_depth;
	   TraceType trace_type = ValidateOptions::getTracteType(po);
	   for (const pair<StateID, size_t> & final : finals) {
	   vector<StateTransition> trans_temp;
	   double robust_temp = 0.;
	   size_t new_cost = computeLasso(po, final, trans_temp, param_no, robust_temp);
	   // Clear data if the new path is shorter than the others.
	   if (new_cost < cost) {
	   cost = new_cost;
	   robustness_val = 0.;
	   trans.clear();
	   }
	   robustness_val += robust_temp;
	   trans.insert(trans.begin(), trans_temp.begin(), trans_temp.end());
	   }

	   sort(trans.begin(), trans.end());
	   trans.erase(unique(trans.begin(), trans.end()), trans.end());

	   return cost;
	   }*/

	/**
	 * @brief checkFull conduct model check with both trying to reach and with cycle detection.
	 * @param[in] witness of the shortest path
	 * @param[in] robustness_val robustness of the shortest paths
	 * @param param_no number of parametrization to test
	 * @param BFS_bound current bound on depth
	 * @return  the Cost value for this parametrization
	 */
	tuple<size_t, double, vector<StateTransition> >  checkFinite(const size_t bfs_bound, const TraceType trace_type, const Levels & parametrization) {
		tuple<size_t, double, vector<StateTransition> > result;
		
		CheckerSetting settings;
		settings.bfs_bound = bfs_bound;
		settings.bound_type = BoundType::min;
		settings.mark_initals = true;
		SynthesisResults s_results = model_checker->conductCheck(settings, parametrization);

		if (trace_type != TraceType::none) {
			searcher->findWitnesses(s_results, settings, parametrization);
			computer->compute(s_results, searcher->getTransitions(), settings, parametrization);
			get<1>(result) = computer->getRobustness();
			if (trace_type != TraceType::wit)
				get<2>(result) = searcher->getTransitions();
		}

		get<0>(result) = s_results.isAccepting() ? s_results.getLowerBound() : INF;
		return result;
	}
};