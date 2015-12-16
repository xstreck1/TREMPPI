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

#include "automaton_structure.hpp"
#include "unparametrized_structure.hpp"
#include "transition_system_interface.hpp"

/// State of the product - same as the state of UKS but put together with a BA state.

struct ProdState : public AutomatonStateProperty<TSTransitionProperty>, public TSStateProperty<TSTransitionProperty> 
{
	const StateID _KS_ID; ///< ID of an original KS state this one is built from
	const StateID _BA_ID; ///< ID of an original BA state this one is built from

	/// Simple filler, assigns values to all the variables
	ProdState(const StateID ID, const StateID KS_ID, const StateID BA_ID, const bool is_initial, const bool is_final, const Levels & levels);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Holds a product structure - the one that is used in coloring procedure.
///
/// This is the final step of construction - a structure that is acutally used during the computation. For simplicity, it copies data from its predecessors (BA and UKS).
/// @attention States of product are indexed as (BA_state_count * KS_state_ID + BA_state_ID) - e.g. if 3-state BA state ((1,0)x(1)) would be at position 3*1 + 1 = 4.
///
/// ProductStructure data can be set only from the ProductBuilder object.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ProductStructure : public AutomatonInterface<ProdState>, public TSInterface<ProdState> 
{
	GROUNDED(ProductStructure);

	pair<size_t, size_t> _sizes;

	//
	StateID computeID(const StateID KS_ID, const StateID BA_ID) const;

	//
	string getString(const StateID ID) const;
};