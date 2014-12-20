#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/definitions.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief An auxiliary class to the ProductStructure
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VisitStorage {
	vector<size_t> states; ///< Vector of states that correspond to those of Product Structure and store visiting data.
	size_t cost; ///< The current cost value;

public:
	/* */
	VisitStorage(const size_t state_count) {
		// Create states
		states.resize(state_count, INF);
		cost = 0;
	}

	VisitStorage() = default; ///< Empty constructor for an empty storage.

	/**
	 * Sets all values for all the states to zero. Allocated memory remains.
	 */
	void reset() {
		states.assign(states.size(), INF);
		cost = 0;
	}

	/**
	* Called when a state is found
	* @param ID	index of the state
	* @return  true if there was an actuall update
	*/
	inline bool update(const StateID ID) {
		// If nothing is new return false
		if (isFound(ID))
			return false;
		// Add new parameters and return true
		states[ID] = cost;
		return true;
	}

	/**
	 * Return true if the state would be updated, false otherwise.
	 * @return  true if there would be an update
	 */
	inline bool isFound(const StateID ID) const {
		return states[ID] != INF;
	}

	/**/
	inline size_t getVisit(const StateID ID) const {
		return states[ID];
	}

	inline size_t getCost() const  {
		return cost;
	}

	inline void incCost() {
		cost++;
	}

	inline bool succeeded() const {
		return cost != INF;
	}

	inline void notFound() {
		cost = INF;
	}
};
