#pragma once

#include <tremppi_common/general/common_functions.hpp>

struct SynthesisResults {
   map<StateID, size_t> found_depth; ///< when a final state was found
   map<size_t, size_t> depths;


   inline bool isAccepting() {
	   return  !found_depth.empty();
   }

   /**
    * @brief derive  information from stored final states
    */
   void derive() {
      for (const pair<StateID, size_t> & state : found_depth) {
         auto it = depths.find(state.second);
         if (it != depths.end()) {
            it->second++;
         } else {
            depths.insert(make_pair(state.second, 1u));
         }
      }
   }

   /**
    * @brief getLowerBound obtain the lowest cost in between all the values
    */
   size_t getUpperBound() const {
      if (depths.empty())
         return INF;
      else
         return (depths.rbegin())->first;
   }

   /**
    * @brief getUpperBound obtain the lowest cost in between all the values
    */
   size_t getLowerBound() const {
      if (depths.empty())
         return INF;
      else
         return depths.begin()->first;
   }

   /**
    * @return final states that are reached exactly within the given depth
    */
   vector<StateID> getFinalsAtDepth(const size_t depth) const {
      vector<StateID> states;

      for (const pair<StateID, size_t> & state : found_depth)
         if (state.second == depth)
            states.push_back(state.first);

      return states;
   }
};
