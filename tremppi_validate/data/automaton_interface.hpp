#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file This file holds abstract basis for a finite automaton described as a graph with states and transitions between the states.
/// Automaton is parametrized by state and state is parametrized by transition. Automaton state is also defined here.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "graph_interface.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief A state structure enhanced with information whether the state is final and/or initial.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Transition>
struct AutomatonStateProperty : public StateProperty<Transition> {
   bool initial; ///< True if the state is initial.
   bool final; ///< True if this state is final.

   /**
    * Adds information if the state is final or initial, passes the rest.
    */
   inline AutomatonStateProperty<Transition>(const bool _initial, const bool _final, const StateID ID)
      : StateProperty<Transition>(ID), initial(_initial), final(_final) { }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  \brief Interface for all the classes that represent a Buchi automaton. Buchi automaton is based on a GraphInterface.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename StateT>
class AutomatonInterface : public virtual GraphInterface<StateT> {
protected:
   vector<StateID> initial_states; ///< Vector with indexes of initial states (in this case only the first state).
   vector<StateID> final_states; ///< Vector with indexes of final states of the BA.
   AutType my_type; ///< Type of this automaton (influences verification procedure).

public:

	/**
    * For a given state find out whether it is marked as final.
	 *
	 * @param ID	state to test
	 *
	 * @return	true if the state is final
	 */
	virtual inline bool isFinal(const StateID ID) const {
        return GraphInterface<StateT>::states[ID].final;
	}

	/**
	 * For given state find out if it is marked as initial.
	 *
	 * @param ID	state to test
	 *
	 * @return	true if the state is initial
	 */
	virtual inline bool isInitial(const StateID ID) const {
        return GraphInterface<StateT>::states[ID].initial;
	}

	/**
	 * Get IDs of all states that are marked as final.
	 *
	 * @return vector of final states' IDs
	 */
	virtual inline const vector<StateID> & getFinalStates() const {
		return final_states;
	}

	/**
	 * Get IDs of all states that are marked as initial.
	 *
	 * @return vector of initial states' IDs
	 */
	virtual inline const vector<StateID> & getInitialStates() const {
		return initial_states;
	}

   /**
    * @brief getMyType obtain type of the automaton
    *
    * @return value of AutType enum
    */
   virtual inline AutType getMyType() const {
      return my_type;
   }
};

