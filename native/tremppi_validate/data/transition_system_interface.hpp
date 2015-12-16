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

#include "graph_interface.hpp"

/// Structure with constraints on a transition within a TS
struct TransConst 
{
	ParamNo _param_no : 59; ///< id of the parameter
	bool _req_dir : 1; ///< true for increase, false for decrease
	ActLevel _req_value : 4; ///< value of the component that's being questioned
};

struct StayConst
{
	ParamNo _param_no : 60; ///< id of the parameter
	ActLevel _req_value : 4; ///< value of the component that's being questioned
};

/// Storing a single transition to neighbour state together with its transition function.
struct TSTransitionProperty : public TransitionProperty 
{
	TransConst _trans_const;

	inline TSTransitionProperty(const StateID t_ID, TransConst trans_const)
		: TransitionProperty(t_ID), _trans_const(trans_const) {}
};

/// State having specie levels attached.
template <typename Transition>
struct TSStateProperty : public virtual StateProperty<Transition> 
{
	Levels _levels; ///< Species_level[i] = activation level of specie i.
	vector<StateID> _loops; ///< States with the Same KS ID, but different BA that are possible targets
	std::vector<StayConst> _stay_const; ///< Requirements to stay in each of the components

	inline TSStateProperty(const StateID ID, const Levels& levels)
		: StateProperty<Transition>(ID), _levels(levels) 
	{
		_stay_const.resize(levels.size()); ///< Pre-alocate
	} 
};

/// Transition system has states labeled with activity levels and transitions labeleld with constraints.
template<typename StateT>
struct TSInterface : public virtual GraphInterface<StateT> {};
