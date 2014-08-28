#pragma once

#include "product_structure.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Creates a final ProductStructure that is used as a template for the synthesis procedure, as a product of ParametrizedStructure and AutomatonStructure.
///
/// ProductBuilder creates the an automaton corresponding to the synchronous product of BA and KS.
/// @attention States of product are indexed as (BA_state_ID * KS_state_count + KS_state_ID) - e.g. if 4-state KS, state ((1,0)x(1)) would be at position 4*1 + 1 = 2.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ProductBuilder {
	/* Create the copy of Unparametrized structure with the states of product with the given BA_ID. */
	void createSubspace(const StateID BA_ID, ProductStructure & product) const {
		for (const StateID KS_ID : crange(product.getStructure().getStateCount())) {
			StateID ID = product.getProductID(KS_ID, BA_ID);
			const Levels& levels = product.getStructure().getStateLevels(KS_ID);
			product.states.push_back(ProdState(ID, KS_ID, BA_ID, false, false, levels));
		}
	}

	/**
	 * Label, as initial, only those states that have any outgoing transition (from all possible initials). 
	 * In the same way, label, as accepting, under the condition that it's not a terminal automaton, the accepting states. 
	 */
	void relabel(const StateID BA_ID, ProductStructure & product) const {
		if (product.getAutomaton().isInitial(BA_ID)) {
			for (const StateID KS_ID : crange(product.getStructure().getStateCount())) {
				StateID ID = product.getProductID(KS_ID, BA_ID);
				// If there's a way to leave the state
				if ((product.states[ID].transitions.size() + product.states[ID].loops.size()) > 0) {
					product.initial_states.push_back(ID);
					product.states[ID].initial = true;
				}
			}
		}
		if (product.getAutomaton().isFinal(BA_ID)) {
			for (const StateID KS_ID : crange(product.getStructure().getStateCount())) {
				StateID ID = product.getProductID(KS_ID, BA_ID);
				// If there's a way to leave the state
				if ((product.states[ID].transitions.size() + product.states[ID].loops.size()) > 0 || (product.getAutomaton().getMyType() == BA_finite)) {
					product.final_states.push_back(ID);
					product.states[ID].final = true;
				}
			}
		}
	}

	/**
	 * Create state of the product as a combination of a single BA and a single UKS state
	 * @param BA_ID	source in the BA
	 * @param transition_count	value which counts the transition for the whole product, will be filled
	 */
	void addSubspaceTransitions(const StateID BA_ID, const size_t trans_no, ProductStructure & product) const {
		const UnparametrizedStructure & structure = product.getStructure();
		const AutomatonStructure & automaton = product.getAutomaton();
		StateID BA_target = automaton.getTargetID(BA_ID, trans_no);

		// List through the states that are allowed by the constraint
		Gecode::DFS<ConstraintParser> search(automaton.getTransitionConstraint(BA_ID, trans_no));
		while (ConstraintParser *result = search.next()) {
			StateID KS_ID = structure.getID(result->getSolution());
			StateID ID = product.getProductID(KS_ID, BA_ID);

			// Add all the trasient combinations for the kripke structure
			if (!automaton.isStableRequired(BA_ID, trans_no)) {
				for (const size_t trans_no : crange(structure.getTransitionCount(KS_ID))) {
					const StateID KS_target = product.getStructure().getTargetID(KS_ID, trans_no);
					const TransConst & trans_const = product.getStructure().getTransitionConst(KS_ID, trans_no);
					product.states[ID].transitions.push_back({ product.getProductID(KS_target, BA_target), trans_const });
				}
			}
			// Add a self-loop
			if (!automaton.isTransientRequired(BA_ID, trans_no)) 
				product.states[ID].loops.push_back(product.getProductID(KS_ID, BA_target));

			delete result;
		}
	}


public:
	/**
	 * Create the the synchronous product of the provided BA and UKS.
	 */
	ProductStructure buildProduct(UnparametrizedStructure  _structure, AutomatonStructure  _automaton) const {
		ProductStructure product(move(_structure), move(_automaton));

		// Creates states and their transitions
		for (size_t BA_ID = 0; BA_ID < product.getAutomaton().getStateCount(); BA_ID++) {
			// Create that what relates to this BA state
			createSubspace(BA_ID, product);
			for (const size_t trans_no : crange(product.getAutomaton().getTransitionCount(BA_ID))) 
				addSubspaceTransitions(BA_ID, trans_no, product);
			relabel(BA_ID, product);
		}
		return product;
	}
};