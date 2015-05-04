#pragma once

#include <tremppi_common/network/data_info.hpp>

#include "../data/property_info.hpp"
#include "unparametrized_structure.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Creates a UnparametrizedStructure based on the regulatory network
///
/// UnparametrizedStructureBuilder creates the UnparametrizedStructure from the model data.
/// States are read from the basic structure and passed to the unparametrized structure, then the transitions are added.
/// Each transition is supplemented with a label - mask of transitive values and the its function ID.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace UnparametrizedStructureBuilder {
	// Test wheather the current state corresponds to the requirements put on values of the specified species.
	bool testRegulators(const vector<Levels> & requirements, const Levels & state_levels);

	// Obtain index of the function that might lead to the specified state based on current activation levels of the species and target state.
	ParamNo getActiveFunction(const map<size_t, vector<Levels> > & requirements, const Levels & state_levels);

	/**
	 * Creates a vector of index differences between neighbour states in each direction.
	 * Value is difference of the indexes between two states that were generated as a cartesian product.
	 * Differences are caused by the way the states are generated.
	 */
	vector<size_t> computeJumps(const Levels & comp_range);

	// Create the states from the model and fill the structure with them.
	UnparametrizedStructure buildStructure(const RegInfos & _reg_infos, const tuple<Levels, Levels, Levels> & bounds);
};