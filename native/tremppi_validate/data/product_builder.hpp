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

#include "product_structure.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Creates a final ProductStructure that is used as a template for the analysis procedure, as a product of UnparametrizedStructure and AutomatonStructure.
///
/// ProductBuilder creates the an automaton corresponding to the synchronous product of BA and KS.
/// @attention States of product are indexed as (BA_state_ID * KS_state_count + KS_state_ID) - e.g. if 4-state KS, state ((1,0)x(1)) would be at position 4*1 + 1 = 2.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ProductBuilder 
{
	/*
	 * Create the copy of Unparametrized structure with the states of product with the given BA_ID. 
	 */
	void createSubspace(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, const StateID BA_ID, ProductStructure & product);

	/**
	 * Add transitions to a single state matching the BA transition.
	 */
	void addStateTransitions(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, const StateID s_BA_ID, const StateID s_KS_ID, const AutTransitionion & BA_transition, ProductStructure & product);

	/**
	 *
	 */
	bool satisfiesMeasurements(const Levels & levels, const MeasurementsCons & measurements_cons);


	/**
	 * Add transitions to all states matching the BA transition.
	 */
	void addSubspaceTransitions(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, const StateID BA_ID, const size_t trans_no, ProductStructure & product);

	/*
	 * @return true	iff the the constrains are met for all the components
	 */ 
	bool satisfiesDeltas(const UnparametrizedStructure & structure, const CompID s_ID, const CompID t_ID, const DeltasCons & deltas_cons) ;

	/**
	 * Create the the synchronous product of the provided BA and UKS.
	 */
	void buildProduct(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, ProductStructure & product);
};