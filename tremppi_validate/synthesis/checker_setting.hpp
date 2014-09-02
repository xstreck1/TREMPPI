#pragma once

#include "../construction/product_structure.hpp"

class CheckerSettings {
public:
   vector<StateID> initial_states;
   vector<StateID> final_states;
   BoundType bound_type;
   TraceType trace_type;
   size_t bfs_bound;
   bool mark_initals;
   size_t minimal_count;

   CheckerSettings() : bound_type(BoundType::inf), trace_type(TraceType::none), bfs_bound(INF), mark_initals(false), minimal_count(1) { }

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

   inline size_t getBound() const {
      return bfs_bound;
   }

   inline bool markInitials() const {
      return mark_initals;
   }

   inline size_t getMinCount() const {
      return minimal_count;
   }
};

