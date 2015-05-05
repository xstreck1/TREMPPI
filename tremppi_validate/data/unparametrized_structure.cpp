#include "unparametrized_structure.hpp"

void UnparametrizedStructure::setBounds(const tuple<Levels, Levels, Levels> & bounds) {
	_bounds = bounds;
}


StateID UnparametrizedStructure::computeID(const Levels & levels) const {
	StateID result = 0;
	size_t factor = 1;

	for (size_t lvl_no = 0; lvl_no < levels.size(); lvl_no++) {
		result += (levels[lvl_no] - get<0>(_bounds)[lvl_no]) * factor;
		factor *= (get<2>(_bounds)[lvl_no]);
	}

	return result;
}

const tuple<Levels, Levels, Levels> & UnparametrizedStructure::getBounds() const {
	return _bounds;
}