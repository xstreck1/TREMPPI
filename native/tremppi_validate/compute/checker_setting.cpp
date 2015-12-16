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

#include "checker_setting.hpp"

CheckerSetting::CheckerSetting() : bound_type(BoundType::bt_inf), bfs_bound(INF), circ(false), stable(false), minimal_count(1) { }

bool CheckerSetting::isInitial(const StateID ID, const ProductStructure & product) const 
{
	if (initial_states.empty())
		return product._states[ID]._initial;
	else
		return (find(initial_states.begin(), initial_states.end(), ID) != initial_states.end());
}

bool CheckerSetting::isFinal(const StateID ID, const ProductStructure & product) const 
{
	if (final_states.empty())
		return product._states[ID]._final;
	else
		return (find(final_states.begin(), final_states.end(), ID) != final_states.end());
}

const vector<StateID> CheckerSetting::getInitials(const ProductStructure & product) const 
{
	if (initial_states.empty())
		return product.getInitialStates();
	else
		return initial_states;
}

const vector<StateID> CheckerSetting::getFinals(const ProductStructure & product) const 
{
	if (final_states.empty())
		return product.getFinalStates();
	else
		return final_states;
}
