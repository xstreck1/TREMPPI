#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/definitions.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief An auxiliary class to the ProductStructure and stores colors and possibly predecessors for individual states of the product during the computation.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ColorStorage {	
   vector<bool> states; ///< Vector of states that correspond to those of Product Structure and store coloring data.

public:
	/**
    * Constructor allocates necessary memory for further usage (this memory is not supposed to be freed until endo of the computation).
	 * Every state has predecessors and succesors allocated for EVERY other state, this consumes memory but benefits the complexity of operations.
	 * @param states_count	number of states the structure the data will be saved for has
	 */
   ColorStorage(const ProductStructure & product) {
      // Create states
      states.resize(product.getStateCount(), false);
	}

	ColorStorage() = default; ///< Empty constructor for an empty storage.

   /**
    * Function adds values from specified source without explicitly copying them, only through bitwise or (storages must be equal).
    */
   void addFrom(const ColorStorage & other) {

      auto m_state_it = states.begin();
      auto o_state_it = other.states.begin();

      while (m_state_it != states.end()) {
         // Copy from paramset
         *m_state_it = (*m_state_it == true) || (*o_state_it == true);

         m_state_it++; o_state_it++;
      }
   }

	/**
	 * Sets all values for all the states to zero. Allocated memory remains.
	 */ 
	void reset() {
      states.assign(states.size(),false);
	}

	/**
    * Add passed colors to the state.
	 * @param ID	index of the state to fill
	 * @param parameters to add - if empty, add all, otherwise use bitwise or
    * @return  true if there was an actuall update
	 */
   inline bool update(const StateID & col) {
		// If nothing is new return false
      if (states[col])
			return false;
		// Add new parameters and return true
      states[col] = true;
		return true;
	}

	/**
	 * Return true if the state would be updated, false otherwise.
    * @return  true if there would be an update
	 */
   inline bool isFound(const StateID & col) {
      if (states[col])
			return false;
		else
			return true;
	}

   /**
	 * @param ID	index of the state to ask for parameters
    * @return  parameters assigned to the state
	 */
    inline bool getColor(const StateID ID) const {
        return states[ID];
	}
};
