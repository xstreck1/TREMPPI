#pragma once

#include "../data/product_structure.hpp"
#include "../data/property_info.hpp"

class CheckerSetting {
public:
   vector<StateID> initial_states;
   vector<StateID> final_states;
   BoundType bound_type;
   size_t bfs_bound;
   bool circ; ///< True if we are looking only for cycles (i.e. init==accepting)
   bool stable; ///< True if we are looking only for paths with a stable last state.
   size_t minimal_count;

   CheckerSetting();

   bool isInitial(const StateID ID, const ProductStructure & product) const;

   bool isFinal(const StateID ID, const ProductStructure & product) const;

   const vector<StateID> & getInitials(const ProductStructure & product) const;

   const vector<StateID> & getFinals(const ProductStructure & product) const;
};

