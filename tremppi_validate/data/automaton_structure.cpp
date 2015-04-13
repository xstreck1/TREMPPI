#include "automaton_structure.hpp"

AutTransitionion::~AutTransitionion() {
	DEL_IF_EXISTS(trans_constr)
}

AutTransitionion::AutTransitionion(AutTransitionion && other) : TransitionProperty{ other.target_ID } {
	trans_constr = other.trans_constr;
	other.trans_constr = nullptr;
}

AutTransitionion::AutTransitionion(const StateID target_ID, ConstraintParser * _trans_constr)
	: TransitionProperty( target_ID ), trans_constr(_trans_constr) {}

AutState::AutState(const StateID ID, const bool final, vector<PathCons> _path_cons)
	: AutomatonStateProperty<AutTransitionion>((ID == 0), final, ID), path_cons(move(_path_cons)) {}

AutomatonStructure::AutomatonStructure() {
	init_constr = nullptr;
	acc_constr = nullptr;
}

AutomatonStructure::~AutomatonStructure() {
	DEL_IF_EXISTS(init_constr);
	DEL_IF_EXISTS(acc_constr);
}

AutomatonStructure & AutomatonStructure::operator=(AutomatonStructure && other) {
	states = move(other.states);
	my_type = other.my_type;
	initial_states = move(other.initial_states);
	final_states = move(other.final_states);
	init_constr = other.init_constr;
	other.init_constr = nullptr;
	acc_constr = other.acc_constr;
	other.acc_constr = nullptr;
	return *this;
}

AutomatonStructure::AutomatonStructure(AutomatonStructure && other) {
	states = move(other.states);
	my_type = other.my_type;
	initial_states = move(other.initial_states);
	final_states = move(other.final_states);
	init_constr = other.init_constr;
	other.init_constr = nullptr;
	acc_constr = other.acc_constr;
	other.acc_constr = nullptr;
}

void AutomatonStructure::AutomatonStructure::addState(const StateID ID, const bool final, vector<PathCons> path_cons) {
	states.push_back(AutState{ ID, final, move(path_cons) });
	if (ID == 0) {
		initial_states.push_back(ID);
	}
	if (final) {
		final_states.push_back(ID);
	}
}

void AutomatonStructure::addTransition(const StateID ID, AutTransitionion transition) {
	states[ID].transitions.push_back(move(transition));
}

ConstraintParser * AutomatonStructure::getTransitionConstraint(const StateID ID, const size_t trans_no) const {
	return states[ID].transitions[trans_no].trans_constr;
}

const vector<PathCons>& AutomatonStructure::getPathCons(const StateID ID) const {
	return states[ID].path_cons;
}
