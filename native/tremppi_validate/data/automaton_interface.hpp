/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

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
struct AutomatonStateProperty : public virtual StateProperty<Transition> 
{
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

struct AutomatonInterface : public virtual GraphInterface<StateT> 
{
   AutType _aut_type; ///< Type of this automaton (influences verification procedure).

	/**
	 * Get IDs of all states that are marked as final.
	 *
	 * @return vector of final states' IDs
	 */
	virtual inline vector<StateID> getFinalStates() const 
	{
		vector<StateID> result;
		for (const StateT & state : this->_states) 
		{
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
	virtual inline vector<StateID> getInitialStates() const 
	{
		vector<StateID> result;
		for (const StateT & state : this->_states) 
		{
			if (state._initial) {
				result.push_back(state._ID);
			}
		}
		return result;
	}
};

