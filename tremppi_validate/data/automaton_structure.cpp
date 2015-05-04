#include "automaton_structure.hpp"

AutTransitionion::AutTransitionion(const StateID t_ID, const Configurations & state_constr, const vector<PathCons> & path_constr)
	: TransitionProperty{ t_ID }, _state_constr{ state_constr }, _path_constr{path_constr} {}

AutState::AutState(const StateID ID, const bool is_initial, const bool is_final)
	: AutomatonStateProperty<AutTransitionion>(ID, is_initial, is_final) {}

AutomatonStructure::AutomatonStructure(const AutType aut_type, const Configurations & init_const, const Configurations & acc_constr)
	: _init_constr{ init_const }, _acc_constr(acc_constr) 
{ 
	_aut_type = aut_type;
}

void AutomatonStructure::AutomatonStructure::addState(const StateID ID, const bool is_initial, const bool is_final) {
	_states.push_back(AutState{ ID, is_initial, is_final });
	if (is_initial) {
		_initial_states.push_back(ID);
	}
	if (is_final) {
		_final_states.push_back(ID);
	}
}

void AutomatonStructure::addTransition(const StateID ID, const StateID t_ID, const Configurations & state_constr, const vector<PathCons> & path_constr) {
	_states[ID]._transitions.push_back(AutTransitionion(t_ID, state_constr, path_constr));
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


