#pragma once

#include "succ_func.hpp"
#include "visit_storage.hpp"
#include "checker_setting.hpp"

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
	size_t DFS(const VisitStorage & storage, const StateID ID, const size_t depth, size_t branch) {
		// If this path is no use
		if (storage.getVisit(ID) < depth)
			return branch;

		path[depth] = ID;

		// Store if the state is final or part of another path.
		if ((found[ID] <= depth) || (settings.isFinal(ID, product) && !(settings.circ && depth == 0))) {
			for (size_t step = branch; step < depth; step++) {
				transitions.insert({ path[step], path[step + 1] });
				found[path[step]] = step; // Mark found for given parametrizations
			}
			branch = depth;
		}

		// Continue with the DFS otherwise.
		else if ((depth < max_depth - 1) && !used[ID]) {
			vector<StateID> transports = SuccFunc::broadcastParameters(parametrization, product, ID);

			for (const StateID & succ : transports) {
				branch = std::min(DFS(storage, succ, depth + 1, branch), depth); // Recursive descent with parametrizations passed from the predecessor.
			}
		}

		used[ID] = true;
		return branch;
	}

public:
	/**
	 * Constructor ensures that data objects used within the whole computation process have appropriate size.
	 */
	WitnessSearcher(const ProductStructure & _product) : product(_product) {
		found = vector<size_t>(product.getStateCount(), INF);
		used = vector<bool>(product.getStateCount(), false);
	}

	/**
	 * Function that executes the whole searching process
	 */
	multimap<StateID, StateID> findWitnesses(const CheckerSetting & _settings, const Levels & _parametrization, const VisitStorage & storage) {
		// Preparation
		settings = _settings;
		parametrization = _parametrization;

		found.assign(found.size(), INF);
		used.assign(used.size(), false);
		path = vector<StateID>(storage.getCost(), INF);
		max_depth = storage.getCost();

		for (const auto & init : settings.getInitials(product))
			if (storage.isFound(init))
				DFS(storage, init, 0u, 0u);

		return move(transitions);
	}

	/**
	 * Re-formes the transitions computed during the round into strings.
	 * @return  strings with all transitions for each acceptable parametrization
	 */
	const static string getOutput(const TraceType trace_type, const ProductStructure & product, const size_t max_cost, const multimap<StateID, StateID>  & transitions) {
		string acceptable_paths; // Vector fo actuall data

		set<StateID> current_depth(WHOLE(product.getInitialStates()));
		size_t cost = 1;

		while (max_cost != INF && cost <= max_cost) {
			set<StateID> next_depth;
			for (StateID ID : current_depth) {
				auto range = transitions.equal_range(ID);

				for (auto it = range.first; it != range.second; it++) {
					acceptable_paths.append(product.getString(ID, cost))
						.append(">")
						.append(product.getString(it->second, cost+1))
						.append(";");
					next_depth.insert(it->second);
				}
			}

			current_depth = next_depth;
			cost++;
		}

		// remove last semi-colon
		if (!acceptable_paths.empty())
			acceptable_paths.resize(acceptable_paths.size() - 1);

		return acceptable_paths;
	}
};
