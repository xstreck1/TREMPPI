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

#include "../data/property_info.hpp"

#include "witness_searcher.hpp"
#include "visit_storage.hpp"
#include "model_checker.hpp"
#include "robustness_compute.hpp"
#include "checker_setting.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief STEP 3 - Control class for the computation.
///
/// Manager of the analysis procedure - takes the reference data constructed during previous steps and computes and executes the analysis.
/// Analysis is done in three steps:
///	-# preparation: empties data and starts a new round.
///   -# analysis: computes the coloring, stored in the storage object and adds data to coloring analyzer if needed.
///   -# conclusion: stores additional data and outputs
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AnalysisManager 
{
	const ProductStructure & product;
	WitnessSearcher searcher; ///< Class to build wintesses.
	RobustnessCompute computer; ///< Class to compute robustness.
	const size_t bfs_bound;
	const bool witness;
	const bool robustness; 
	
	/**
	 * @brief standard conduct model check for a lasso
	 */
	tuple<size_t, multimap<StateID, StateID>, double >  standard(const Levels & parametrization);

	/**
	 * @brief finite conduct model check with reachability only
	 */
	tuple<size_t, multimap<StateID, StateID>, double > finite(const Levels & parametrization, const bool stable);

public:
	/**
	 * Constructor builds all the data objects that are used within.
	 */
	AnalysisManager(const ProductStructure & _product, const size_t _bfs_bound, const bool _witness, const bool _robustness);

	/**
	 * The entry point of analysis
	 */
	tuple<size_t, multimap<StateID, StateID>, double >  check(const Levels & parametrization);
};