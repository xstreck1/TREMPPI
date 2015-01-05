#pragma once

#include "graph_interface.hpp"

/// Structure with constraints on a transition within a TS
struct TransConst {
   uint16_t param_no : 15; ///< id of the parameter
   bool req_dir : 1; ///< true for increase, false for decrease
   ActLevel req_value; ///< value of the specie that's being questioned
};

/// Storing a single transition to neighbour state together with its transition function.
struct TSTransitionProperty : public TransitionProperty {
   TransConst trans_const;

   TSTransitionProperty(const StateID target_ID, const uint16_t param_no, const bool req_dir, const ActLevel req_value)
	   : TransitionProperty(target_ID), trans_const({ param_no, req_dir, req_value }) {}
};

/// State having specie levels attached.
struct TSStateProperty : public StateProperty<TSTransitionProperty> {
   Levels levels; ///< Species_level[i] = activation level of specie i.
   vector<StateID> loops;

   TSStateProperty(const StateID ID, const Levels& _species_level)
      : StateProperty<TSTransitionProperty>(ID), levels(_species_level) { } ///< Simple filler, assigns values to all the variables.
};

/// Transition system has states labeled with activity levels and transitions labeleld with constraints.
template<typename StateT>
class TSInterface : public virtual GraphInterface<StateT> {
public:

   /**
    * @brief getTransitionConst returns constrains on a specified transition
    * @param ID of the source state
    * @param trans_no ordinar number of the transition
    * @return reference to the constraints data structure
    */
   virtual inline const TransConst & getTransitionConst(const StateID ID, const size_t trans_no) const {
      return GraphInterface<StateT>::states[ID].transitions[trans_no].trans_const;
   }

   /**
    * @brief getStateLevels return reference to activity levels of this state
    * @param ID of the state
    * @return reference to Levels data structure
    */
   virtual inline const Levels & getStateLevels(const StateID ID) const {
      return GraphInterface<StateT>::states[ID].levels;
   }

   inline const vector<StateID> & getLoops(const StateID ID) const {
       return GraphInterface<StateT>::states[ID].loops;
   }
};
