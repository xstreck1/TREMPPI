#pragma once

#include "unparametrized_structure.hpp"
#include "../data/kinetics.hpp"
#include "../data/parameter_helper.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Creates a UnparametrizedStructure as a composition of a BasicStructure and ParametrizationsHolder.
///
/// UnparametrizedStructureBuilder creates the UnparametrizedStructure from the model data.
/// States are read from the basic structure and passed to the unparametrized structure, then the transitions are added.
/// Each transition is supplemented with a label - mask of transitive values and the its function ID.
/// This expects semantically correct data from BasicStructure and FunctionsStructure.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UnparametrizedStructureBuilder {
	const RegInfos & reg_infos;
	const PropertyAutomaton & property;
	const MyKinetics & kinetics;

	vector<size_t> index_jumps; ///< Holds index differences between two neighbour states in each direction for each ID.
	vector<bool> allowed_states; ///< Masking the states (by IDs) that are allowed by the current experiment

	/**
	 * @return Returns true if the transition may be ever feasible from this state.
	 */
	bool isFeasible(const Levels & parameter_vals, const bool direction, const ActLevel level) {
		for (const ActLevel val : parameter_vals) {
			if (direction) {
				if (val > level) {
					return true;
				}
			}
			else {
				if (val < level) {
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * @brief addTransition add a transition if feasible
	 */
	void addTransition(const StateID source, const StateID target, const CompID ID, const bool direction, const Levels & state_levels, UnparametrizedStructure & structure) {
		// Find out which function is currently active
		const size_t fun_no = getActiveFunction(ID, state_levels);
		// Fill the step size
		const ParamNo step_size = kinetics.components[ID].step_size;
		// Reference target values
		const Levels & parameter_vals = kinetics.components[ID].params[fun_no].target_in_subcolor;

		if (isFeasible(parameter_vals, direction, state_levels[ID]))
			structure.addTransition(source, target, step_size, direction, state_levels[ID], parameter_vals);
	}

	/**
	 * @brief addTransitions   add all transitions for the given ID
	 */
	void addTransitions(const StateID trans_ID, const Levels & state_levels, UnparametrizedStructure & structure) {
		for (const CompID ID : cscope(reg_infos)) {
			// If this value is not the lowest one, add neighbour with lower
			if (state_levels[ID] > structure.mins[ID]) {
				const StateID target_ID = trans_ID - index_jumps[ID];
				if (allowed_states[target_ID])
					addTransition(trans_ID, target_ID, ID, false, state_levels, structure);
			}
			// If this value is not the highest one, add neighbour with higher
			if (state_levels[ID] < structure.maxes[ID]) {
				const StateID target_ID = trans_ID + index_jumps[ID];
				if (allowed_states[target_ID])
					addTransition(trans_ID, target_ID, ID, true, state_levels, structure);
			}
		}
	}

	/**
	 * Test wheather the current state corresponds to the requirements put on values of the specified species.
	 */
	bool testRegulators(const map<CompID, Levels> & requirements, const Levels & state_levels) {
		// Const function
		if (requirements.empty())
			return true;

		// List throught regulating species of the function
		for (auto & regul : requirements)
			if (count(regul.second.begin(), regul.second.end(), state_levels[regul.first]) == 0)
				return false;

		// Return true if all species passed.
		return true;
	}

	/**
	 * Obtain index of the function that might lead to the specified state based on current activation levels of the species and target state.
	 */
	size_t getActiveFunction(const CompID ID, const Levels & state_levels) {
		// Cycle until the function is found
		bool found = false;
		for (const size_t param_no : cscope(kinetics.components[ID].params)) {
			found = testRegulators(kinetics.components[ID].params[param_no].requirements, state_levels);
			if (found)
				return param_no;
		}
		throw runtime_error("Active function in some state not found.");
	}

	/**
	 * Creates a vector of index differences between neighbour states in each direction.
	 * Value is difference of the indexes between two states that were generated as a cartesian product.
	 * Differences are caused by the way the states are generated.
	 */
	void computeJumps(const Levels & range_size) {
		index_jumps.reserve(reg_infos.size());
		// How many far away are two neighbour in the vector
		size_t jump_lenght = 1;
		// Species with higher index cause bigger differences
		for (size_t specie_no : cscope(reg_infos)) {
			index_jumps.emplace_back(jump_lenght);
			jump_lenght *= (range_size[specie_no]);
		}
	}

	/* Prepare the data structure that stores IDs of allowed states. */
	void prepareAllowed(const UnparametrizedStructure & structure, const size_t state_count, const bool init) {
		if (state_count * property.states.size() > structure.states.max_size())
			throw runtime_error("The number of states of the product (" + to_string(state_count * property.states.size()) +
			" is bigger than the maximum of " + to_string(structure.states.max_size()));
		allowed_states.resize(state_count, init);
	}

	// Label, as allowed, those states that satisfy the experiment
	size_t solveConstrains(UnparametrizedStructure & structure) {
		pair<Levels, Levels> bounds = ParameterHelper::getBounds(reg_infos, property);
		structure.mins = bounds.first; structure.maxes = bounds.second;
		rng::transform(structure.maxes, structure.mins, back_inserter(structure.range_size), [](const ActLevel max, const ActLevel min) {return max - min + 1;});

		ConstraintParser * cons_pars = new ConstraintParser(reg_infos.size(), DataInfo::getMaxLevel(reg_infos));
		cons_pars->addBoundaries(bounds.first, false);
		cons_pars->addBoundaries(bounds.second, true);
		cons_pars->applyFormula(DataInfo::getAllNames(reg_infos), property.experiment);
		// Compute distances between neighbours
		computeJumps(structure.range_size);

		// Mark allowed states
		size_t state_count = accumulate(structure.range_size.begin(), structure.range_size.end(), 1, multiplies<size_t>());
		bool all_states = property.experiment == "tt";
		prepareAllowed(structure, state_count, all_states);

		// Conduct search
		if (!all_states) {
			Gecode::DFS<ConstraintParser> search(cons_pars);
			delete cons_pars;
			while (ConstraintParser *result = search.next()) {
				StateID ID = structure.getID(result->getSolution());
				allowed_states[ID] = true;
				delete result;
			}
		}

		return state_count;
	}

public:
	UnparametrizedStructureBuilder(const RegInfos & _reg_infos, const PropertyAutomaton & _property, const MyKinetics & _kinetics)
		: reg_infos(_reg_infos), property(_property), kinetics(_kinetics) {}

	/**
	 * Create the states from the model and fill the structure with them.
	 */
	UnparametrizedStructure buildStructure() {
		UnparametrizedStructure structure;

		// Create states
		size_t state_no = 0;
		const size_t state_count = solveConstrains(structure);
		Levels levels(structure.mins);
		do {
			// Fill the structure with the state
			if (allowed_states[state_no]) {
				structure.addState(state_no, levels);
				addTransitions(state_no, levels, structure);
			}
			else {
				structure.addState(state_no, Levels());
			}
			state_no++;
		} while (iterate(structure.maxes, structure.mins, levels));

		return structure;
	}
};