#pragma once

#include "../data/product_structure.hpp"
#include "../data/property_automaton.hpp"

class CheckerSetting {
public:
   vector<StateID> initial_states;
   vector<StateID> final_states;
   BoundType bound_type;
   size_t bfs_bound;
   bool circ; ///< True if we are looking only for cycles (i.e. init==accepting)
   bool stable; ///< True if we are looking only for paths with a stable last state.
   size_t minimal_count;

   CheckerSetting() : bound_type(BoundType::inf),  bfs_bound(INF), circ(false), stable(false), minimal_count(1) { }

   inline bool isInitial(const StateID ID, const ProductStructure & product) const {
      if (initial_states.empty())
         return product.isInitial(ID);
      else
         return (find(initial_states.begin(),initial_states.end(), ID) != initial_states.end());
   }

   inline bool isFinal(const StateID ID, const ProductStructure & product) const {
      if (final_states.empty())
         return product.isFinal(ID);
      else
         return (find(final_states.begin(),final_states.end(), ID) != final_states.end());
   }

   inline const vector<StateID> & getInitials(const ProductStructure & product) const {
      if (initial_states.empty())
         return product.getInitialStates();
      else
         return initial_states;
   }

   inline const vector<StateID> & getFinals(const ProductStructure & product) const {
      if (final_states.empty())
         return product.getFinalStates();
      else
         return final_states;
   }
};

