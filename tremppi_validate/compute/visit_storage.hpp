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
	VisitStorage(const size_t state_count);

	VisitStorage() = default; ///< Empty constructor for an empty storage.

	/**
	 * Sets all values for all the states to zero. Allocated memory remains.
	 */
	void reset();

	/**
	* Called when a state is found
	* @param ID	index of the state
	* @return  true if there was an actuall update
	*/
	bool update(const StateID ID);

	/**
	 * Return true if the state would be updated, false otherwise.
	 * @return  true if there would be an update
	 */
	bool isFound(const StateID ID) const;

	size_t getVisit(const StateID ID) const;

	size_t getCost() const;

	void incCost();

	bool succeeded() const;

	void notFound();
};
