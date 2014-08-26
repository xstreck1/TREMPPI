#pragma once

#include <tremppi_common/network/model.hpp>
#include "../model/property_automaton.hpp"

#include "witness_searcher.hpp"
#include "output_manager.hpp"
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
   void analyseLasso(const pair<StateID, size_t> & final, vector<StateTransition> & trans, const ParamNo param_no, double & robust, const bool robustness) {
      SynthesisResults results;
      CheckerSettings settings;
      // First find the coloring from the initial states to the given final.
      settings.final_states = {final.first};
      settings.minimize_cost = settings.mark_initals = true;
      settings.param_no = param_no;
      results = model_checker->conductCheck(settings);
      searcher->findWitnesses(results, settings);
      trans = searcher->getTransitions();
      if (robustness) {
         computer->compute(results, searcher->getTransitions(), settings);
         robust = computer->getRobustness();
      }

      // Second find a cycle on the final state.
      settings.mark_initals = false;
      settings.initial_states = {final.first};
      results = model_checker->conductCheck(settings);
      searcher->findWitnesses(results, settings);
      const vector<StateTransition> & trans_ref = searcher->getTransitions();
      trans.insert(trans.begin(), trans_ref.begin(), trans_ref.end());
      if (robustness) {
         computer->compute(results, searcher->getTransitions(), settings);
         robust *= computer->getRobustness();
      }
   }

   /**
    * @brief computeLasso parametrization is know to reach a final state, test that state for a bounded loop.
    */
   size_t computeLasso(const pair<StateID, size_t> & final, vector<StateTransition> & trans, const ParamNo param_no,  double & robust, const size_t BFS_bound, const bool witnesses, const bool robustness) {
      CheckerSettings settings;
      settings.minimize_cost = true;
      settings.param_no = param_no;
      settings.initial_states = settings.final_states = {final.first};
      settings.bfs_bound = BFS_bound == INF ? BFS_bound : (BFS_bound - final.second);

      SynthesisResults results = model_checker->conductCheck(settings);
      const size_t cost = results.getLowerBound() == INF ? INF : results.getLowerBound() + final.second;
      if (results.isAccepting() && (witnesses || robustness))
         analyseLasso(final, trans, param_no, robust, robustness);

      return cost;
   }

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
   size_t checkFull(vector<StateTransition> & trans, double & robustness_val, const ParamNo param_no, const size_t BFS_bound, const bool witnesses, const bool robustness) {
      CheckerSettings settings;
      settings.bfs_bound = BFS_bound;
      settings.param_no = param_no;
      settings.mark_initals = true;
      SynthesisResults results = model_checker->conductCheck(settings);

      size_t cost = INF;
      map<StateID, size_t> finals = results.found_depth;
      for (const pair<StateID, size_t> & final : finals) {
         vector<StateTransition> trans_temp;
         double robust_temp = 0.;
         size_t new_cost = computeLasso(final, trans_temp, param_no, robust_temp, BFS_bound, witnesses, robustness);
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
   }

   /**
    * @brief checkFull conduct model check with both trying to reach and with cycle detection.
    * @param[in] witness of the shortest path
    * @param[in] robustness_val robustness of the shortest paths
    * @param param_no number of parametrization to test
    * @param BFS_bound current bound on depth
    * @param witnesses should compute witnesses
    * @param robustness should compute robustness
    * @return  the Cost value for this parametrization
    */
   size_t checkFinite(vector<StateTransition> & trans, double & robustness_val, const ParamNo param_no,
                      const size_t BFS_bound, const bool witnesses, const bool robustness, const size_t min_acc, const size_t max_acc) {
      CheckerSettings settings;
      settings.param_no = param_no;
      settings.bfs_bound = BFS_bound;
      settings.minimize_cost = true;
      settings.mark_initals = true;
	  settings.minimal_count = min_acc;
      SynthesisResults results = model_checker->conductCheck(settings);

      if ((witnesses || robustness) && results.isAccepting(min_acc, max_acc)) {
         searcher->findWitnesses(results, settings);
         if (robustness)
            computer->compute(results, searcher->getTransitions(), settings);
         robustness_val = robustness ? computer->getRobustness() : 0.;
         if (witnesses)
            trans = searcher->getTransitions();
      }

      return results.isAccepting(min_acc, max_acc) ? results.getLowerBound() : INF;
   }
};