#include "automaton_structure.hpp"

AutTransitionion::AutTransitionion(const StateID t_ID, const Configurations & state_constr, const vector<PathCons> & path_constr)
	: TransitionProperty(t_ID), _state_constr(state_constr), _path_constr(path_constr) {}

//
void AutomatonStructure::setInitContrs(const Configurations & init_constr) {
	_init_constr = init_constr;
}

//
void AutomatonStructure::setAccContrs(const Configurations & acc_constr) {
	_acc_constr = acc_constr;
}

const Configurations & AutomatonStructure::getStateConstr(const StateID ID, const size_t trans_no) const {
	return _states[ID]._transitions[trans_no]._state_constr;
}

const vector<PathCons> & AutomatonStructure::getPathConstr(const StateID ID, const size_t trans_no) const {
	return _states[ID]._transitions[trans_no]._path_constr;
}

const Configurations & AutomatonStructure::getInitConstr() const {
	return _init_constr;
}

const Configurations & AutomatonStructure::getAccConstr() const {
	return _acc_constr;
}

char AutomatonStructure::IDToName(const size_t ID) {
	return static_cast<char>(ID + 65);
}

size_t AutomatonStructure::NameToID(const char name) {
	return static_cast<size_t>(name) - 65u;
}
