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

#include "visit_storage.hpp"
#include "checker_setting.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Main class of the computation - responsible for the CMC procedure.
///
/// ModelChecker class solves the parameter analysis problem by iterative transfer of feasible parametrizations from initial states to final ones.
/// Functions in model checker use many supporting variables and therefore are quite long, it would not make sense to split them, though.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ModelChecker 
{
	/**
	 * @return true if this transition is open for given parametrization
	 */
	bool isOpen(const Levels & parametrization, const TransConst & trans_const);

	/**
	 * @return true if the state is stable in the current parametrization
	 */
	bool isStable(const Levels & parametrization, const std::vector<StayConst> & stay_consts);

	/**
	* @return (vector of reachable targets from ID for this parametrization, is the state stable?)
	*/
	template <class State>
	pair<vector<StateID>, bool> broadcastParameters(const Levels & parametrization, const TSInterface<State> & ts, const StateID ID);

	/**
	*
	*/
	VisitStorage conductCheck(const ProductStructure & product, const CheckerSetting & _settings, const Levels & _parametrization, VisitStorage storage);
};

template <class State>
pair<vector<StateID>, bool> ModelChecker::broadcastParameters(const Levels & parametrization, const TSInterface<State> & ts, const StateID ID) 
{
	vector<StateID> targets;

	// Cycle through all the transition
	for (const size_t trans_no : cscope(ts._states[ID])) 
	{
		// From an update strip all the parameters that can not pass through the transition - color intersection on the transition
		if (isOpen(parametrization, ts._states[ID]._transitions[trans_no]._trans_const)) 
		{
			targets.push_back(ts._states[ID]._transitions[trans_no]._t_ID);
		}
	}

	// Add loops only if there is no transition (even a hell transition) available.
	if (targets.empty() && isStable(parametrization, ts._states[ID]._stay_const))
		return make_pair(ts._states[ID]._loops, true);
	else
		return make_pair(targets, false);
}