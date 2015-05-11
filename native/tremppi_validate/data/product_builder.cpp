#include "product_builder.hpp"

void ProductBuilder::createSubspace(const UnparametrizedStructure & structure, const StateID BA_ID, ProductStructure & product) {
	for (const StateID KS_ID : cscope(structure)) {
		StateID ID = product.computeID(KS_ID, BA_ID);
		const Levels& levels = structure._states[KS_ID]._levels;
		product._states.emplace_back(ProdState(ID, KS_ID, BA_ID, false, false, levels));
	}
}

void ProductBuilder::relabel(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, const StateID BA_ID, ProductStructure & product) {
	if (automaton._states[BA_ID]._initial) {
		Levels levels = min(automaton._init_constr);
		do {
			product._states[product.computeID(structure.computeID(levels), BA_ID)]._initial = true; 
		} while (iterate(automaton._init_constr, levels));
	}
	if (automaton._states[BA_ID]._final) {
		Levels levels = min(automaton._acc_constr);
		do {
			product._states[product.computeID(structure.computeID(levels), BA_ID)]._final = true;
		} while (iterate(automaton._acc_constr, levels));
	}
}

bool ProductBuilder::matchesConstraints(const UnparametrizedStructure & structure, const CompID s_ID, const CompID t_ID, const vector<PathCons>& path_cons) {
	bool result = true;
	
	const Levels & s_levels = structure._states[s_ID]._levels;
	const Levels & t_levels = structure._states[t_ID]._levels;

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

void ProductBuilder::addSubspaceTransitions(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, const StateID BA_ID, const size_t BA_trans_no, ProductStructure & product) {
	// List through the states that are allowed by the constraint
	const auto & BA_transition = automaton._states[BA_ID]._transitions[BA_trans_no];
	Levels levels = min(BA_transition._state_constr);
	do {
		// bool is_ss = static_cast<bool>(solution[solution.size() - 1]);
		// solution.resize(solution.size() - 1);
		StateID KS_ID = structure.computeID(levels);
		StateID ID = product.computeID(KS_ID, BA_ID);

		// Add all the trasient combinations for the kripke structure
		for (const size_t KS_trans_no : cscope(structure._states[KS_ID])) {
			const StateID KS_target = structure._states[KS_ID]._transitions[KS_trans_no]._t_ID;
			const TransConst & trans_const = structure._states[KS_ID]._transitions[KS_trans_no]._trans_const;
			// If the transition does not meet the stability requirements, add transition to hell (infty)
			const StateID t_ID = matchesConstraints(structure, KS_ID, KS_target, BA_transition._path_constr) ? product.computeID(KS_target, BA_transition._t_ID) : INF;
			product._states[ID]._transitions.emplace_back(TSTransitionProperty( t_ID, trans_const ));
		}

		// Add a self-loop
		product._states[ID]._loops.emplace_back(product.computeID(KS_ID, BA_transition._t_ID));
	} while (iterate(BA_transition._state_constr, levels));
}

void ProductBuilder::buildProduct(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, ProductStructure & product) {
	product._sizes = make_pair(structure.size(), automaton.size());
	product._aut_type = automaton._aut_type;

	// Creates states and their transitions
	for (const size_t BA_ID : cscope(automaton)) {
		// Create that what relates to this BA state
		createSubspace(structure, BA_ID, product);
		for (const size_t trans_no : cscope(automaton._states[BA_ID])) {
			addSubspaceTransitions(structure, automaton, BA_ID, trans_no, product);
		}
		relabel(structure, automaton, BA_ID, product);
	}
}