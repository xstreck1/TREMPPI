#include "automaton_structure.hpp"

AutTransitionion::~AutTransitionion() {
	if (trans_constr != nullptr)
		delete trans_constr;
}

AutTransitionion::AutTransitionion(AutTransitionion && other) : TransitionProperty(other.target_ID) {
	trans_constr = other.trans_constr;
	other.trans_constr = nullptr;
}

AutTransitionion::AutTransitionion(const StateID target_ID, ConstraintParser * _trans_constr)
	: TransitionProperty(target_ID), trans_constr(_trans_constr) {}

AutState::AutState(const StateID ID, const bool final, const vector<StateID> _stables)
	: AutomatonStateProperty<AutTransitionion>((ID == 0), final, ID), stables(move(_stables)) {}

AutomatonStructure & AutomatonStructure::operator=(AutomatonStructure && other) {
	states = move(other.states);
	my_type = other.my_type;
	initial_states = move(other.initial_states);
	final_states = move(other.final_states);
	return *this;
}

void AutomatonStructure::AutomatonStructure::addState(const StateID ID, const bool final, const vector<StateID> stables) {
	states.push_back({ ID, final, move(stables) });
	if (ID == 0)
		initial_states.push_back(ID);
	if (final)
		final_states.push_back(ID);
}

void AutomatonStructure::addTransition(const StateID ID, AutTransitionion transition) {
	states[ID].transitions.push_back(move(transition));
}

ConstraintParser * AutomatonStructure::getTransitionConstraint(const StateID ID, const size_t trans_no) const {
	return states[ID].transitions[trans_no].trans_constr;
}

const vector<CompID>& AutomatonStructure::getStables(const StateID ID) const {
	return states[ID].stables;
}
