#include "unparametrized_structure.hpp"

void UnparametrizedStructure::addState(const StateID ID, Levels species_level) {
	GraphInterface<TSStateProperty>::states.emplace_back(TSStateProperty(ID, move(species_level)));
}

/**
* @param ID	add data to the state with this IS
* Add a new transition to the source specie, containg necessary edge labels for the CMC
*/
void UnparametrizedStructure::addTransition(const StateID ID, const StateID target_ID, const uint16_t param_no, const bool req_dir, const ActLevel req_level) {
	GraphInterface<TSStateProperty>::states[ID].transitions.push_back(TSTransitionProperty(target_ID, param_no, req_dir, req_level));
}

StateID UnparametrizedStructure::getID(const Levels & levels) const {
	StateID result = 0;
	size_t factor = 1;

	for (size_t lvl_no = 0; lvl_no < levels.size(); lvl_no++) {
		result += (levels[lvl_no] - mins[lvl_no]) * factor;
		factor *= (range_size[lvl_no]);
	}

	return result;
}
