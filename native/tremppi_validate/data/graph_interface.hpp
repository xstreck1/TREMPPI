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

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/definitions.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file This file holds abstract basis for a graph with states and transitions between the states.
/// Graph is parametrized by state and state is parametrized by transition. Basic state and transition are also defined here.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief This is just a very simple basis for a transition in a graph.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct TransitionProperty 
{
	/// Unique ID of the state.
	const StateID _t_ID;
	inline TransitionProperty(const StateID t_ID)
		: _t_ID(t_ID) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief This is just a very simple basis for a state of any graph.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Transition>
struct StateProperty 
{
	/// Unique ID of the state.
	const StateID _ID;
	/// Graph or automaton transitions, basically it is an edge with a label.
	vector<Transition> _transitions;

	inline StateProperty(const StateID ID) : _ID(ID) {}

	inline size_t size() const 
	{
		return _transitions.size();
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Interface for all the classes that represent a directed graph. Transitions are expected to be stored within their source state structure.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename StateT, typename TransitionT = decltype(declval<StateT>()._transitions.front())>
struct GraphInterface 
{
	vector<StateT> _states; ///< Vector holding states of the graph.

	/**
	 * Obtains number of states of the graph.
	 * @return integer with size of the graph
	 */
	inline size_t size() const 
	{
		return _states.size();
	}
	/**
	 * Returns given state as a string.
	 * @param ID	ID of the state to turn into the string
	 * @return	given state as a string
	 */
	inline string getString(const StateID ID) const 
	{
		return to_string(ID);
	}
};