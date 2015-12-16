/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

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

namespace UnparametrizedStructureBuilder 
{
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
	void buildStructure(const RegInfos & reg_infos, UnparametrizedStructure & structure);
};