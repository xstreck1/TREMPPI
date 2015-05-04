#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/definitions.hpp>
#include "transition_system_interface.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Complete Kripke structure with only possible transitions containing encoded kinetic functions.
///
/// UnparametrizedStructure stores states of the Kripke structure created from the model together with labelled transitions.
/// Each transition contains a function that causes it with explicit enumeration of values from the function that are transitive.
/// To easily search for the values in the parameter bitmask, step_size of the function is added
/// - that is the value saying how many bits of mask share the the same value for the function.
/// UnparametrizedStructure data can be set only from the UnparametrizedStructureBuilder object.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UnparametrizedStructure : public TSInterface<TSStateProperty> {
	tuple<Levels, Levels, Levels> _bounds;

public:
	UnparametrizedStructure(tuple<Levels, Levels, Levels> bounds);
	NO_COPY_SHORT(UnparametrizedStructure);
	DEFAULT_MOVE(UnparametrizedStructure);

	// Add a new state, only with ID and levels
	void addState(const StateID ID, const Levels & levels);

	// Add a new transition to the source specie, containg necessary edge labels for the CMC
	void addTransition(const StateID ID, const StateID target_ID, const ParamNo fun_no, const bool req_dir, const ActLevel req_level);

	// convert the levels to a state ID
	StateID getID(const Levels & levels) const;
};
