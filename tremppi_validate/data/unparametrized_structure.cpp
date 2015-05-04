#include "unparametrized_structure.hpp"

UnparametrizedStructure::UnparametrizedStructure(const tuple<Levels, Levels, Levels> & bounds) 
	: _bounds{ bounds } {}

void UnparametrizedStructure::addState(const StateID ID, const Levels & levels) {
	GraphInterface<TSStateProperty>::_states.emplace_back(TSStateProperty(ID, levels));
}

void UnparametrizedStructure::addTransition(const StateID ID, const StateID target_ID, const ParamNo fun_no, const bool req_dir, const ActLevel req_level) {
	GraphInterface<TSStateProperty>::_states[ID]._transitions.push_back(TSTransitionProperty(target_ID, fun_no, req_dir, req_level));
}

StateID UnparametrizedStructure::getID(const Levels & levels) const {
	StateID result = 0;
	size_t factor = 1;

	for (size_t lvl_no = 0; lvl_no < levels.size(); lvl_no++) {
		result += (levels[lvl_no] - get<0>(_bounds)[lvl_no]) * factor;
		factor *= (get<2>(_bounds)[lvl_no]);
	}

	return result;
}