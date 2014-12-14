#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/definitions.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief An auxiliary class to the ProductStructure and stores colors and possibly predecessors for individual states of the product during the computation.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ColorStorage {
	vector<size_t> states; ///< Vector of states that correspond to those of Product Structure and store coloring data.

public:
	/**
	* Constructor allocates necessary memory for further usage (this memory is not supposed to be freed until endo of the computation).
	* Every state has predecessors and succesors allocated for EVERY other state, this consumes memory but benefits the complexity of operations.
	* @param states_count	number of states the structure the data will be saved for has
	*/
	ColorStorage(const ProductStructure & product) {
		// Create states
		states.resize(product.getStateCount(), INF);
	}

	ColorStorage() = default; ///< Empty constructor for an empty storage.

	/**
	 * Sets all values for all the states to zero. Allocated memory remains.
	 */
	void reset() {
		states.assign(states.size(), INF);
	}

	/**
	* Called when a state is found
	* @param ID	index of the state
	* @return  true if there was an actuall update
	*/
	inline bool update(const StateID ID, const size_t BFS_level) {
		// If nothing is new return false
		if (isFound(ID))
			return false;
		// Add new parameters and return true
		states[ID] = BFS_level;
		return true;
	}

	/**
	 * Return true if the state would be updated, false otherwise.
	 * @return  true if there would be an update
	 */
	inline bool isFound(const StateID ID) const {
		return states[ID] != INF;
	}
};
