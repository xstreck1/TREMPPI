#pragma once

#include "coloring_func.hpp"
#include "color_storage.hpp"
#include "synthesis_results.hpp"
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
	const ColorStorage & storage; ///< Constant storage with the actuall data.
	CheckerSetting settings; ///< Setup for the process.
	Levels parametrization;

	vector<StateTransition>  transitions; ///< Acutall storage of the transitions found - transitions are stored by parametrizations numbers in the form (source, traget).

	vector<StateID> path; ///< Current path of the DFS with the final vertex on 0 position.
	size_t max_depth; ///< Maximal level of recursion that is possible (maximal Cost in this round).

	/// This structure stores "already tested" paramsets for a state.
	struct Marking {
		size_t succeeded; ///< Mask of the parametrizations that are
		size_t busted; ///< Mask of the parametrizations that are guaranteed to not find a path in (Cost - depth) steps.
	};
	vector<Marking> markings; ///< Actuall marking of the states.

	/**
	 * Storest transitions in the form (source, target) within the transitions vector, for the path from the final vertex to the one in the current depth of the DFS procedure.
	 * @param which   mask of the parametrizations that allow currently found path
	 */
	void storeTransitions(const size_t depth, size_t & last_branch) {
		// Go from the end till the lastly reached node
		for (size_t step = last_branch; step < depth; step++) {
			transitions.push_back(StateTransition(path[step], path[step + 1]));
			markings[path[step + 1]].succeeded = step + 1; // Mark found for given parametrizations
		}
		last_branch = depth;
	}

	/**
	 * Searching procedure itself. This method is called recursivelly based on the depth of the search and passes current parametrizations based on the predecessors.
	 * @param ID   ID of the state visited
	 */
	size_t DFS(const StateID ID, const size_t depth, size_t last_branch) {
		// If this path is no use
		if (markings[ID].busted <= depth || markings[ID].succeeded < depth)
			return last_branch;

		// Store if the state is final or part of another path.
		if (depth != 0 || !settings.isFinal(ID, product))
			markings[ID].busted = depth;
		path[depth] = ID;
		if (settings.isFinal(ID, product) && depth != 0)
			storeTransitions(depth, last_branch);
		else if (markings[ID].succeeded >= depth && markings[ID].succeeded != INF)
			storeTransitions(depth, last_branch);
		// Continue with the DFS otherwise.
		else if (depth < max_depth){
			vector<StateID> transports;

			if (ColoringFunc::broadcastParameters(parametrization, product.getStructure(), product.getKSID(ID)).empty())
				transports = product.getLoops(ID);
			else
				transports = ColoringFunc::broadcastParameters(parametrization, product, ID);


			for (const StateID & succ : transports) {
				last_branch = std::min(DFS(succ, depth + 1, last_branch), depth); // Recursive descent with parametrizations passed from the predecessor.
			}
		}
		return last_branch;
	}

public:
	/**
	 * Constructor ensures that data objects used within the whole computation process have appropriate size.
	 */
	WitnessSearcher(const ProductStructure & _product, const ColorStorage & _storage) : product(_product), storage(_storage) {
		markings.resize(product.getStateCount());
	}

	/**
	 * Function that executes the whole searching process
	 */
	void findWitnesses(const SynthesisResults & results, const CheckerSetting & _settings, const Levels & _parametrization) {
		// Preparation
		settings = _settings;
		parametrization = _parametrization;
		transitions.clear();

		// Search paths from all the final states
		for (const pair<size_t, size_t> depth : results.depths) {
			path = vector<StateID>(depth.first + 1, INF); // Currently needs one more space for the transition to a final state after the last measurement.
			markings.assign(markings.size(), { INF, INF });
			max_depth = depth.first;
			auto inits = settings.getInitials(product);
			settings.final_states = results.getFinalsAtDepth(max_depth);
			for (const auto & init : inits)
				if (storage.isFound(init))
					DFS(init, 0u, 0u);
		}
	}

	/**
	 * @return  transitions for each parametrizations in the form (source, target)
	 */
	const vector<StateTransition> & getTransitions() const {
		return transitions;
	}

	/**
	 * Re-formes the transitions computed during the round into strings.
	 * @return  strings with all transitions for each acceptable parametrization
	 */
	const static string getOutput(const TraceType trace_type, const ProductStructure & product, const vector<StateTransition> & transitions) {
		string acceptable_paths; // Vector fo actuall data
		// Cycle throught the parametrizations
		if (!transitions.empty()) { // Test for emptyness of the set of transitions
			acceptable_paths = "{";
			// Reformes based on the user request
			for (const StateTransition & trans : transitions){
				acceptable_paths.append(to_string(trans.first)).append(">").append(to_string(trans.second)).append(",");
				// acceptable_paths.append(product.getString(trans.first)).append(">").append(product.getString(trans.second)).append(",");
			}
			acceptable_paths.back() = '}';
		}
		return acceptable_paths;
	}
};
