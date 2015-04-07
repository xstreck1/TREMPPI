#include "model_checker.hpp"

/**
* @return true if this transition is open for given parametrization
*/
bool ModelChecker::isOpen(const Levels & parametrization, const TransConst & trans_cost) {
	if (trans_cost.req_dir)
		return parametrization[trans_cost.param_no] > trans_cost.req_value;
	else
		return parametrization[trans_cost.param_no] < trans_cost.req_value;
}

/**
*
*/
VisitStorage ModelChecker::conductCheck(const ProductStructure & product, const CheckerSetting & _settings, const Levels & _parametrization, VisitStorage storage) {
	storage.reset();

	vector<StateID> updates = _settings.getInitials(product);
	if (!_settings.circ) {
		for (const StateID ID : updates) {
			storage.update(ID);
		}
	}
	vector<StateID> next_updates; ///< Updates that are sheduled forn the next round.
	bool sat = false;

	// While there are updates, pass them to succesing vertices
	while (!updates.empty()) {
		storage.incCost();
		for (const StateID ID : updates) {
			// Get passed states and remove those already found
			vector<StateID> transports; bool stable;
			tie(transports, stable) = broadcastParameters(_parametrization, product, ID);
			auto trans_end = remove_if(WHOLE(transports), [&storage](const StateID t_ID) {return storage.isFound(t_ID); });

			for (auto next_it = begin(transports); next_it != trans_end; next_it++) {
				storage.update(*next_it);
			}

			next_updates.insert(end(next_updates), begin(transports), trans_end);

			// Don't update when looking for a loop
			if (!(_settings.circ && storage.getCost() == 1)) {
				if (_settings.isFinal(ID, product) && (stable || !_settings.stable))
					sat = true;
			}
		}
		if ((sat && _settings.bound_type == BoundType::min) || (_settings.bfs_bound <= storage.getCost() && _settings.bound_type == BoundType::step))
			updates.clear();
		else
			updates = move(next_updates);

	};

	if (!sat)
		storage.notFound();
	return storage;
}
