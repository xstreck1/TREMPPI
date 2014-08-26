/*
* Copyright (C) 2012-2013 - Adam Streck
* This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
* ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
* ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
* For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
*/

#ifndef PARSYBONE_SPACE_SOLVER_INCLUDED
#define PARSYBONE_SPACE_SOLVER_INCLUDED

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/definitions.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief An RAII object for searching Gecode solutions.
/// Space must implement getSolution() function that returns a vector of variables.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename SpaceType, typename VarType = int>
class SpaceSolver final {
	SpaceType * space; ///< Holds some version of the Gecode::Space
	DFS<SpaceType> * dfs; ///< Eventually holds a DFS for the Space

public:
	SpaceSolver(SpaceType * space) : dfs(nullptr) {
		this->space = space;
	}

	NO_COPY_SHORT(SpaceSolver);

	SpaceSolver(SpaceSolver && other) {
		this->space = other.space;
		other.space = nullptr;
		this->dfs = other.dfs;
		other.dfs = nullptr;
	}

	SpaceSolver & operator=(SpaceSolver && other) {
		this->space = other.space;
		other.space = nullptr;
		this->dfs = other.dfs;
		other.dfs = nullptr;
		return *this;
	}

	~SpaceSolver() {
		if (space != nullptr) // Deleted after all solutions have been found
			delete space;
		if (dfs != nullptr) // Deleted before any search even started
			delete dfs;
	}
	
	// Provides access to the space, space functions can be called this way
	// @return pointer to the space that's being held.
	SpaceType * operator->() const {
		if (dfs != nullptr)
			throw runtime_error("Trying to access Gecode Space in a space solver that has already started solving.");
		return space;
	}

	// @return	a next solution if available and an empty vector if not
	vector<VarType> next() {
		vector<VarType> result;

		// Initialize the search in the first call
		if (dfs == nullptr) 
			dfs = new DFS<SpaceType>(space);

		// Obtain the new space
		delete space;
		space = dfs->next();

		// C to C++ representation
		if (space == NULL) 
			space = nullptr;
		
		// Get the result if there's any
		if (space != nullptr)
			result = space->getSolution();
	
		return result;
	}
};

#endif
