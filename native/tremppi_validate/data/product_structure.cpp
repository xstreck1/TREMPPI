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

#include "product_structure.hpp"

ProdState::ProdState(const StateID ID, const StateID KS_ID, const StateID BA_ID, const bool is_initial, const bool is_final, const Levels & levels)
	: StateProperty(ID), AutomatonStateProperty(ID, is_initial, is_final), TSStateProperty<TSTransitionProperty>(ID, levels), _KS_ID(KS_ID), _BA_ID(BA_ID) {}


StateID ProductStructure::computeID(const StateID KS_ID, const StateID BA_ID) const 
{
	return (BA_ID * _sizes.first + KS_ID);
}


string ProductStructure::getString(const StateID ID) const 
{
	string label;

	for (const ActLevel lev : this->_states[ID]._levels)
		label += to_string(lev);

	label += char(this->_states[ID]._BA_ID + 65);

	return label;
}
