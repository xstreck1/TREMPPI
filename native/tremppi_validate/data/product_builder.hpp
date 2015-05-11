#pragma once

#include "product_structure.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Creates a final ProductStructure that is used as a template for the analysis procedure, as a product of UnparametrizedStructure and AutomatonStructure.
///
/// ProductBuilder creates the an automaton corresponding to the synchronous product of BA and KS.
/// @attention States of product are indexed as (BA_state_ID * KS_state_count + KS_state_ID) - e.g. if 4-state KS, state ((1,0)x(1)) would be at position 4*1 + 1 = 2.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace ProductBuilder {
	/*
	 * Create the copy of Unparametrized structure with the states of product with the given BA_ID. 
	 */
	void createSubspace(const UnparametrizedStructure & structure, const StateID BA_ID, ProductStructure & product);

	/**
	 * Label, as initial, only those states that have any outgoing transition (from all possible initials).
	 * In the same way, label, as accepting, under the condition that it's not a terminal automaton, the accepting states.
	 */
	void relabel(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, const StateID BA_ID, ProductStructure & product);

	/**
	 * Create state of the product as a combination of a single BA and a single UKS state
	 * @param BA_ID	source in the BA
	 * @param transition_count	value which counts the transition for the whole product, will be filled
	 */
	void addSubspaceTransitions(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, const StateID BA_ID, const size_t trans_no, ProductStructure & product);

	/*
	 * @return true	iff the the constrains are met for all the components
	 */ 
	bool matchesConstraints(const UnparametrizedStructure & structure, const CompID s_ID, const CompID t_ID, const vector<PathCons>& path_cons) ;

	/**
	 * Create the the synchronous product of the provided BA and UKS.
	 */
	void buildProduct(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, ProductStructure & product);
};