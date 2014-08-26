#pragma once

#include "color_storage.hpp"
#include "coloring_func.hpp"
#include "synthesis_results.hpp"
#include "checker_setting.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Main class of the computation - responsible for the CMC procedure.
///
/// ModelChecker class solves the parameter synthesis problem by iterative transfer of feasible parametrizations from initial states to final ones.
/// Functions in model checker use many supporting variables and therefore are quite long, it would not make sense to split them, though.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModelChecker {
   // Information
   const ProductStructure & product; ///< Product on which the computation will be conducted.
   CheckerSettings settings; ///< Setup for the process.

   // Coloring storage
   ColorStorage & storage; ///< Class that actually stores colors during the computation.
   // ColorStorage next_round_storage; ///< Class that stores updated colors for next round (prevents multiple transitions through one BFS round).
   vector<StateID> updates; ///< Set of states that need to spread their updates.
   vector<StateID> next_updates; ///< Updates that are sheduled forn the next round.

   // BFS boundaries
   size_t BFS_level; ///< Number of current BFS level during coloring, starts from 0, meaning 0 transitions.
   SynthesisResults results;

   /**
    * From the source distribute its parameters and newly colored neighbours shedule for update.
    * @param ID	ID of the source state in the product
    * @param parameters	parameters that will be distributed
    */
   void transferUpdates(const StateID ID) {
      // Get passed colors, unique for each sucessor
      vector<StateID> transports;

      if (ColoringFunc::broadcastParameters(settings.getParamNo(), product.getStructure(), product.getKSID(ID)).empty())
         transports = product.getLoops(ID) ;
      else
         transports = ColoringFunc::broadcastParameters(settings.getParamNo(), product, ID);

      // For all passed values make update on target
      for (const StateID trans : transports) {
         // If something new is added to the target, schedule it for an update
         if (storage.isFound(trans)) {
            // Determine what is necessary to update
            storage.update(trans);
            next_updates.push_back(trans);
         }
      }
   }

   /**
    * Main coloring function - passes parametrizations from newly colored states to their neighbours.
    * Executed as an BFS - in rounds.
    */
   void doColoring() {
      StateID ID = updates.back();
      updates.pop_back();

      // Check if this is not the last round
      if (settings.isFinal(ID, product) && storage.getColor(ID))
         results.found_depth.insert({ID, BFS_level});

      transferUpdates(ID);

      // If there this round is finished, but there are still paths to find
      if (updates.empty() && (BFS_level < settings.getBound())) {
		  if (settings.mimizeCost() && results.isAccepting(settings.minimal_count, INF))
			  return;
         updates = move(next_updates);
         BFS_level++; // Increase level
      }
   }

   /**
    * @brief prepareObjects   create empty space in the employed objects
    */
   void prepareObjects() {
      storage.reset();
      next_updates.clear(); // Ensure emptiness of the next round
      BFS_level = 0;
      results = SynthesisResults();
   }

   /**
    * @brief initiateCheck initiate data for the check based on the settings
    */
   void initiateCheck() {
      updates = settings.getInitials(product);
      if (settings.markInitials())
         for (const StateID init_ID : updates)
            storage.update(init_ID);
   }

public:
   ModelChecker(const ProductStructure & _product, ColorStorage & _storage) : product(_product), storage(_storage) {
   }

   /**
    * Start a new coloring round for cycle detection from a single state.
    */
   SynthesisResults conductCheck(const CheckerSettings & _settings) {
      settings = _settings;
      prepareObjects();
      initiateCheck();

      // While there are updates, pass them to succesing vertices
      do  {
         doColoring();
      } while (!updates.empty());

      results.derive();
      return results;
   }
};
