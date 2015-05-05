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
struct AutomatonStateProperty : public virtual StateProperty<Transition> {
   bool _initial; ///< True if the state is initial.
   bool _final; ///< True if this state is final.

   /**
    * Adds information if the state is final or initial, passes the rest.
    */
   inline AutomatonStateProperty<Transition>(const StateID ID, const bool is_initial, const bool is_final)
	   : StateProperty<Transition>(ID), _initial(is_initial), _final(is_final) { }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  \brief Interface for all the classes that represent a Buchi automaton. Buchi automaton is based on a GraphInterface.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename StateT>
class AutomatonInterface : public virtual GraphInterface<StateT> {
protected:
   AutType _aut_type; ///< Type of this automaton (influences verification procedure).

public:
	virtual inline void setInitial(const StateID ID) {
		_states[ID]._initial = true;
	}

	virtual inline void setFinal(const StateID ID) {
		_states[ID]._final = true;
	}

	virtual inline void setType(const AutType aut_type) {
		_aut_type = aut_type;
	}

	/**
     * For a given state find out whether it is marked as final.
	 *
	 * @param ID	state to test
	 *
	 * @return	true if the state is final
	 */
	virtual inline bool isFinal(const StateID ID) const {
        return GraphInterface<StateT>::_states[ID]._final;
	}

	/**
	 * For given state find out if it is marked as initial.
	 *
	 * @param ID	state to test
	 *
	 * @return	true if the state is initial
	 */
	virtual inline bool isInitial(const StateID ID) const {
        return GraphInterface<StateT>::_states[ID]._initial;
	}

	/**
	 * Get IDs of all states that are marked as final.
	 *
	 * @return vector of final states' IDs
	 */
	virtual inline vector<StateID> getFinalStates() const {
		vector<StateID> result;
		for (const StateT & state : _states) {
			if (state._final) {
				result.push_back(state._ID);
			}
		}
		return result;
	}

	/**
	 * Get IDs of all states that are marked as initial.
	 *
	 * @return vector of initial states' IDs
	 */
	virtual inline vector<StateID> getInitialStates() const {
		vector<StateID> result;
		for (const StateT & state : _states) {
			if (state._initial) {
				result.push_back(state._ID);
			}
		}
		return result;
	}

   /**
    * @brief getAutType obtain type of the automaton
    *
    * @return value of AutType enum
    */
   virtual inline AutType getAutType() const {
      return _aut_type;
   }
};

