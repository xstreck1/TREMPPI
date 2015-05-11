#include "witness_searcher.hpp"

size_t WitnessSearcher::DFS(const VisitStorage & storage, const StateID ID, const size_t depth, size_t branch) {
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
		vector<StateID> transports = ModelChecker::broadcastParameters(parametrization, product, ID).first;

		for (const StateID & succ : transports) {
			branch = std::min(DFS(storage, succ, depth + 1, branch), depth); // Recursive descent with parametrizations passed from the predecessor.
		}
	}

	used[ID] = true;
	return branch;
}

WitnessSearcher::WitnessSearcher(const ProductStructure & _product) : product(_product) {
	found = vector<size_t>(product.size(), INF);
	used = vector<bool>(product.size(), false);
}

/**
* Function that executes the whole searching process
*/

multimap<StateID, StateID> WitnessSearcher::findWitnesses(const CheckerSetting & _settings, const Levels & _parametrization, const VisitStorage & storage) {
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

const string WitnessSearcher::getOutput(const ProductStructure & product, const size_t max_cost, const multimap<StateID, StateID>& transitions) {
	string acceptable_paths; // Vector fo actuall data

	vector<StateID> current_depth = product.getInitialStates();
	size_t cost = 1;

	while (max_cost != INF && cost < max_cost) {
		vector<StateID> next_depth;
		for (StateID ID : current_depth) {
			auto range = transitions.equal_range(ID);

			for (auto it = range.first; it != range.second; it++) {
				acceptable_paths.append(product.getString(ID) + "-" + to_string(cost))
					.append(">")
					.append(product.getString(it->second) + "-" + to_string(cost+1))
					.append(";");
				next_depth.emplace_back(it->second);
			}
		}

		current_depth = makeUnique(next_depth);
		cost++;
	}

	// remove last semi-colon
	if (!acceptable_paths.empty())
		acceptable_paths.resize(acceptable_paths.size() - 1);

	return acceptable_paths;
}
