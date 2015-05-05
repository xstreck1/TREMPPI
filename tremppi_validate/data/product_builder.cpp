#include "product_builder.hpp"

void ProductBuilder::createSubspace(const UnparametrizedStructure & structure, const StateID BA_ID, ProductStructure & product) {
	for (const StateID KS_ID : cscope(structure)) {
		StateID ID = product.getProductID(KS_ID, BA_ID);
		const Levels& levels = structure.getState(KS_ID)._levels;
		product.addState(ProdState(ID, KS_ID, BA_ID, false, false, levels));
	}
}

void ProductBuilder::relabel(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, const StateID BA_ID, ProductStructure & product) {
	if (automaton.getState(BA_ID)._initial) {
		Levels levels = min(automaton.getInitConstr());
		while (iterate(automaton.getInitConstr(), levels)) {
			product.setInitial(product.getProductID(structure.computeID(levels), BA_ID));
		}
	}
	if (automaton.getState(BA_ID)._final) {
		Levels levels = max(automaton.getInitConstr());
		while (iterate(automaton.getInitConstr(), levels)) {
			product.setFinal(product.getProductID(structure.computeID(levels), BA_ID));
		}
	}
}

bool ProductBuilder::matchesConstraints(const UnparametrizedStructure & structure, const CompID s_ID, const CompID t_ID, const vector<PathCons>& path_cons) {
	bool result = true;
	
	const Levels & s_levels = structure.getState(s_ID)._levels;
	const Levels & t_levels = structure.getState(t_ID)._levels;

	for (const CompID ID : cscope(path_cons)) {
		switch (path_cons[ID]) {
		case PathCons::pc_up:
			result &= t_levels[ID] >= s_levels[ID];
			break;
		case PathCons::pc_down:
			result &= t_levels[ID] <= s_levels[ID];
			break;
		case PathCons::pc_stay:
			result &= t_levels[ID] == s_levels[ID];
			break;
		}
	}

	return result;
}

void ProductBuilder::addSubspaceTransitions(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, const StateID BA_ID, const size_t trans_no, ProductStructure & product) {
	StateID BA_target = automaton.getState(BA_ID)._transitions[trans_no]._t_ID;

	// List through the states that are allowed by the constraint
	Levels levels = min(automaton.getStateConstr(BA_ID, trans_no));
	while (iterate(automaton.getStateConstr(BA_ID, trans_no), levels)) {
		// bool is_ss = static_cast<bool>(solution[solution.size() - 1]);
		// solution.resize(solution.size() - 1);
		StateID KS_ID = structure.computeID(levels);
		StateID ID = product.getProductID(KS_ID, BA_ID);

		// Add all the trasient combinations for the kripke structure
		for (const size_t trans_no : crange(structure.getTransitionCount(KS_ID))) {
			const StateID KS_target = structure.getState(KS_ID)._transitions[trans_no]._t_ID;
			const TransConst & trans_const = structure.getState(KS_ID)._transitions[trans_no]._trans_const;
			// If the transition does not meet the stability requirements, add transition to hell (infty)
			const StateID t_ID = matchesConstraints(structure, KS_ID, KS_target, automaton.getPathConstr(BA_ID, trans_no)) ? product.getProductID(KS_target, BA_target) : INF;
			product.addTransition(ID, TSTransitionProperty( t_ID, trans_const ));
		}

		// Add a self-loop
		product.addLoop(ID, product.getProductID(KS_ID, BA_target));
	}
}

void ProductBuilder::buildProduct(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, ProductStructure & product) {
	product._sizes = make_pair(structure.size(), automaton.size());

	// Creates states and their transitions
	for (const size_t BA_ID : cscope(automaton)) {
		// Create that what relates to this BA state
		createSubspace(structure, BA_ID, product);
		for (const size_t trans_no : cscope(automaton.getState(BA_ID))) {
			addSubspaceTransitions(structure, automaton, BA_ID, trans_no, product);
		}
		relabel(structure, automaton, BA_ID, product);
	}
}