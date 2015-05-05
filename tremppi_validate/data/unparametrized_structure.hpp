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

using USTransition = TSTransitionProperty;
using USState = TSStateProperty<TSTransitionProperty>;

class UnparametrizedStructure : public TSInterface<USState> {
	tuple<Levels, Levels, Levels> _bounds;

public:
	GROUNDED(UnparametrizedStructure);

	void setBounds(const tuple<Levels, Levels, Levels> & bounds);
	
	// convert the levels to a state ID
	StateID computeID(const Levels & levels) const;

	const tuple<Levels, Levels, Levels> & getBounds() const;
};
