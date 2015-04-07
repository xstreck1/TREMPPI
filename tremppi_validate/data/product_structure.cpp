#include "product_structure.hpp"

ProdTransitionion::ProdTransitionion(const StateID _target_ID, const TransConst & _trans_const)
	: TransitionProperty(_target_ID), trans_const(_trans_const) {}

ProdState::ProdState(const StateID ID, const StateID _KS_ID, const StateID _BA_ID, const bool initial, const bool final, const Levels & _species_level)
	: AutomatonStateProperty<ProdTransitionion>(initial, final, ID), KS_ID(_KS_ID), BA_ID(_BA_ID), levels(_species_level) {}

ProductStructure::ProductStructure(UnparametrizedStructure _structure, AutomatonStructure _automaton) : structure(move(_structure)), automaton(move(_automaton)) {
	my_type = _automaton.getMyType();
}

ProductStructure& ProductStructure::operator=(ProductStructure && other) {
	structure = move(other.structure);
	automaton = move(other.automaton);
	states = move(other.states);
	my_type = other.my_type;
	initial_states = move(other.initial_states);
	final_states = move(other.final_states);
	return *this;
}

const UnparametrizedStructure & ProductStructure::getStructure() const {
	return structure;
}

const AutomatonStructure & ProductStructure::getAutomaton() const {
	return automaton;
}

StateID ProductStructure::getProductID(const StateID KS_ID, const StateID BA_ID) const {
	return (BA_ID * structure.getStateCount() + KS_ID);
}

StateID ProductStructure::getBAID(const StateID ID) const {
	return states[ID].BA_ID;
}

StateID ProductStructure::getKSID(const StateID ID) const {
	return states[ID].KS_ID;
}

const string ProductStructure::getString(const StateID ID) const {
	string label;

	for (const ActLevel lev : states[ID].levels)
		label += to_string(lev);

	label += char(getBAID(ID) + 65);

	return label;
}

const string ProductStructure::getString(const StateID ID, const size_t cost) const {
	string label;

	for (const ActLevel lev : states[ID].levels)
		label += to_string(lev);
	label += "-" + to_string(cost);

	return label;
}
