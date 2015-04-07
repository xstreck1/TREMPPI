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
	friend class UnparametrizedStructureBuilder;
	Levels maxes; ///< Maximal activity levels of the components.
	Levels mins; ///< Minimal activity levels of the components.
	Levels range_size; ///< Differences between the two.

public:
	UnparametrizedStructure() = default;
	UnparametrizedStructure(UnparametrizedStructure &&) = default;
	UnparametrizedStructure(const UnparametrizedStructure &) = delete;
	UnparametrizedStructure& operator=(const UnparametrizedStructure &) = delete;
	UnparametrizedStructure& operator=(UnparametrizedStructure && other) {
		states = move(other.states);
		return *this;
	}

	/**
	  * Add a new state, only with ID and levels
	  */
	void addState(const StateID ID, Levels species_level);

	/**
	 * @param ID	add data to the state with this IS
	 * Add a new transition to the source specie, containg necessary edge labels for the CMC
	 */
	void addTransition(const StateID ID, const StateID target_ID, const uint16_t param_no, const bool req_dir, const ActLevel req_level);

	StateID getID(const Levels & levels) const;
};
