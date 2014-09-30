#include "product_builder.hpp"

void ProductBuilder::createSubspace(const StateID BA_ID, ProductStructure & product) const {
	for (const StateID KS_ID : crange(product.getStructure().getStateCount())) {
		StateID ID = product.getProductID(KS_ID, BA_ID);
		const Levels& levels = product.getStructure().getStateLevels(KS_ID);
		product.states.push_back(ProdState(ID, KS_ID, BA_ID, false, false, levels));
	}
}

void ProductBuilder::relabel(const StateID BA_ID, ProductStructure & product) const {
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

void ProductBuilder::addSubspaceTransitions(const StateID BA_ID, const size_t trans_no, ProductStructure & product) const {
	const UnparametrizedStructure & structure = product.getStructure();
	const AutomatonStructure & automaton = product.getAutomaton();
	StateID BA_target = automaton.getTargetID(BA_ID, trans_no);

	// List through the states that are allowed by the constraint
	Gecode::DFS<ConstraintParser> search(automaton.getTransitionConstraint(BA_ID, trans_no));
	while (ConstraintParser *result = search.next()) {
		auto solution = result->getSolution();
		bool is_ss = static_cast<bool>(solution[solution.size() - 1]);
		solution.resize(solution.size() - 1);
		StateID KS_ID = structure.getID(solution);
		StateID ID = product.getProductID(KS_ID, BA_ID);

		if (is_ss) {
			// Add all the trasient combinations for the kripke structure
			for (const size_t trans_no : crange(structure.getTransitionCount(KS_ID))) {
				const StateID KS_target = product.getStructure().getTargetID(KS_ID, trans_no);
				const TransConst & trans_const = product.getStructure().getTransitionConst(KS_ID, trans_no);
				product.states[ID].transitions.push_back({ product.getProductID(KS_target, BA_target), trans_const });
			}
		}
		else {
			// Add a self-loop
			product.states[ID].loops.push_back(product.getProductID(KS_ID, BA_target));
		}

		delete result;
	}
}

ProductStructure ProductBuilder::buildProduct(UnparametrizedStructure  _structure, AutomatonStructure  _automaton) const {
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