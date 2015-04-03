#pragma once

#include <tremppi_common/network/data_info.hpp>

#include "../data/property_automaton.hpp"
#include "unparametrized_structure.hpp"

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
	const PropertyAutomaton & property_automaton;

	vector<size_t> index_jumps; ///< Holds index differences between two neighbour states in each direction for each ID.

	/**
	 * @brief addTransition add a transition if feasible
	 */
	void addTransition(const StateID source, const StateID target, const CompID ID, const bool direction, const Levels & state_levels, UnparametrizedStructure & structure);

	/**
	 * @brief addTransitions   add all transitions for the given ID
	 */
	void addTransitions(const StateID trans_ID, const Levels & state_levels, UnparametrizedStructure & structure);
	/**
	 * Test wheather the current state corresponds to the requirements put on values of the specified species.
	 */
	bool testRegulators(const vector<Levels> & requirements, const Levels & state_levels);

	/**
	 * Obtain index of the function that might lead to the specified state based on current activation levels of the species and target state.
	 */
	size_t getActiveFunction(const CompID ID, const Levels & state_levels);

	/**
	 * Creates a vector of index differences between neighbour states in each direction.
	 * Value is difference of the indexes between two states that were generated as a cartesian product.
	 * Differences are caused by the way the states are generated.
	 */
	void computeJumps(const Levels & range_size);

	// Label, as allowed, those states that satisfy the experiment
	size_t solveConstrains(UnparametrizedStructure & structure);

public:
	UnparametrizedStructureBuilder(const RegInfos & _reg_infos, const PropertyAutomaton & _property);

	/**
	 * Create the states from the model and fill the structure with them.
	 */
	UnparametrizedStructure buildStructure();
};