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

#include "visit_storage.hpp"
#include "checker_setting.hpp"
#include "model_checker.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Class for search of transitions belonging to shortest time series paths.
///
/// @attention Always finds witnesses only for the paths of minimal cost.
///
/// Class executes a search through the synthetized space in order to find transitions included in shortest paths for every parametrization.
/// Procedure is supposed to be first executed and then it can provide results.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class WitnessSearcher 
{
	const ProductStructure & product; ///< Product reference for state properties.

	CheckerSetting settings;
	Levels parametrization;

	multimap<StateID, StateID> transitions; ///< Acutall storage of the transitions found - transitions are stored by parametrizations numbers in the form (source, traget).
	vector<StateID> path; ///< Current path of the DFS with the final vertex on 0 position.
	size_t max_depth; ///< Maximal level of recursion that is possible (maximal Cost in this round).
	vector<size_t> found; ///< Used for noting when a state was found (depth).
	vector<bool> used; ///< Labelled true if the state was used in the search.

	/**
	 * Searching procedure itself. This method is called recursivelly based on the depth of the search and passes current parametrizations based on the predecessors.
	 * @param ID   ID of the state visited
	 */
	size_t DFS(const VisitStorage & storage, const StateID ID, const size_t depth, size_t branch);

public:
	/**
	 * Constructor ensures that data objects used within the whole computation process have appropriate size.
	 */
	WitnessSearcher(const ProductStructure & _product);

	/**
	 * Function that executes the whole searching process
	 */
	multimap<StateID, StateID> findWitnesses(const CheckerSetting & _settings, const Levels & _parametrization, const VisitStorage & storage);

	/**
	 * Re-formes the transitions computed during the round into strings.
	 * @return  strings with all transitions for each acceptable parametrization
	 */
	const static string getOutput(const ProductStructure & product, const size_t max_cost, const multimap<StateID, StateID>  & transitions);
};
