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
class WitnessSearcher {
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
