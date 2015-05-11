#include "unparametrized_structure_builder.hpp"

bool UnparametrizedStructureBuilder::testRegulators(const vector<Levels> & requirements, const Levels & state_levels) {
	// Const function
	if (requirements.empty())
		return true;

	// List throught regulating species of the function
	for (const CompID ID : cscope(state_levels))
		if (find(WHOLE(requirements[ID]), state_levels[ID]) == end(requirements[ID]))
			return false;

	// Return true if all species passed.
	return true;
}

ParamNo UnparametrizedStructureBuilder::getActiveFunction(const map<size_t, vector<Levels> > & requirements,  const Levels & state_levels) {
	// Cycle until the function is found
	bool found = false;
	for (const auto & req : requirements) {
		found = testRegulators(req.second, state_levels);
		if (found)
			return req.first;
	}
	throw runtime_error("Active function in some state not found.");
}

vector<size_t> UnparametrizedStructureBuilder::computeJumps(const Levels & comp_range) {
	vector<size_t> result(comp_range.size());
	// How many far away are two neighbour in the vector
	size_t jump_lenght = 1;
	// Species with higher index cause bigger differences
	for (CompID ID : cscope(comp_range)) {
		result[ID] = jump_lenght;
		jump_lenght *= (comp_range[ID]);
	}

	return result;
}

void UnparametrizedStructureBuilder::buildStructure(const RegInfos & reg_infos, UnparametrizedStructure & structure) {
	const size_t state_count = accumulate(WHOLE(get<2>(structure._bounds)), static_cast<ActLevel>(1), multiplies<ActLevel>());
	const vector<size_t> jumps = computeJumps(get<2>(structure._bounds));

	StateID s_ID = 0;
	Levels levels(get<0>(structure._bounds));
	do {
		// Fill the structure with the state
		structure._states.emplace_back(USState(s_ID,levels));
		// Add transitions from the state under each dimension
		for (const CompID c_ID : cscope(jumps)) {
			const ParamNo fun_no = getActiveFunction(reg_infos[c_ID].requirements, levels);
			// If this value is not the lowest one, add neighbour with lower
			if (levels[c_ID] > get<0>(structure._bounds)[c_ID]) {
				const StateID t_ID = s_ID - jumps[c_ID];
				structure._states[s_ID]._transitions.emplace_back(USTransition(t_ID, TransConst{ fun_no, false, levels[c_ID] }));
			}
			// If this value is not the highest one, add neighbour with higher
			if (levels[c_ID] < get<1>(structure._bounds)[c_ID]) {
				const StateID t_ID = s_ID + jumps[c_ID];
				structure._states[s_ID]._transitions.emplace_back(USTransition(t_ID, TransConst{ fun_no, true, levels[c_ID] }));
			}
		}
		s_ID++;
	} while (iterate(get<1>(structure._bounds), get<0>(structure._bounds), levels));
}
