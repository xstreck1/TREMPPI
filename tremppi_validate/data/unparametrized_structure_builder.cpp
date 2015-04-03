#include "unparametrized_structure_builder.hpp"

#include <tremppi_common/network/constraint_parser.hpp>

void UnparametrizedStructureBuilder::addTransition(const StateID source, const StateID target, const CompID ID, const bool direction, const Levels & state_levels, UnparametrizedStructure & structure) {
	// Find out which function is currently active
	const size_t fun_no = getActiveFunction(ID, state_levels);

	structure.addTransition(source, target, fun_no, direction, state_levels[ID]);
}

void UnparametrizedStructureBuilder::addTransitions(const StateID trans_ID, const Levels & state_levels, UnparametrizedStructure & structure) {
	for (const CompID ID : cscope(reg_infos)) {
		// If this value is not the lowest one, add neighbour with lower
		if (state_levels[ID] > structure.mins[ID]) {
			const StateID target_ID = trans_ID - index_jumps[ID];
			addTransition(trans_ID, target_ID, ID, false, state_levels, structure);
		}
		// If this value is not the highest one, add neighbour with higher
		if (state_levels[ID] < structure.maxes[ID]) {
			const StateID target_ID = trans_ID + index_jumps[ID];
			addTransition(trans_ID, target_ID, ID, true, state_levels, structure);
		}
	}
}

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


size_t UnparametrizedStructureBuilder::getActiveFunction(const CompID ID, const Levels & state_levels) {
	// Cycle until the function is found
	bool found = false;
	for (const auto & req : reg_infos[ID].requirements) {
		found = testRegulators(req.second, state_levels);
		if (found)
			return req.first;
	}
	throw runtime_error("Active function in some state not found.");
}

void UnparametrizedStructureBuilder::computeJumps(const Levels & range_size) {
	index_jumps.reserve(reg_infos.size());
	// How many far away are two neighbour in the vector
	size_t jump_lenght = 1;
	// Species with higher index cause bigger differences
	for (size_t specie_no : cscope(reg_infos)) {
		index_jumps.emplace_back(jump_lenght);
		jump_lenght *= (range_size[specie_no]);
	}
}

size_t UnparametrizedStructureBuilder::solveConstrains(UnparametrizedStructure & structure) {
	pair<Levels, Levels> bounds = PropertyHelper::getBounds(reg_infos, property_automaton);
	structure.mins = bounds.first; structure.maxes = bounds.second;
	rng::transform(structure.maxes, structure.mins, back_inserter(structure.range_size), [](const ActLevel max, const ActLevel min) {return max - min + 1; });

	// Compute distances between neighbours
	computeJumps(structure.range_size);

	size_t state_count = accumulate(structure.range_size.begin(), structure.range_size.end(), 1, multiplies<size_t>());

	return state_count;
}

UnparametrizedStructureBuilder::UnparametrizedStructureBuilder(const RegInfos & _reg_infos, const PropertyAutomaton & _property_automaton)
	: reg_infos(_reg_infos), property_automaton(_property_automaton) {}

UnparametrizedStructure UnparametrizedStructureBuilder::buildStructure() {
	UnparametrizedStructure structure;

	// Create states
	StateID ID = 0;
	const size_t state_count = solveConstrains(structure);
	Levels levels(structure.mins);
	do {
		// Fill the structure with the state
		structure.addState(ID, levels);
		addTransitions(ID, levels, structure);

		ID++;
	} while (iterate(structure.maxes, structure.mins, levels));

	return structure;
}