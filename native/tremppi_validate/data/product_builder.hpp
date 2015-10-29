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