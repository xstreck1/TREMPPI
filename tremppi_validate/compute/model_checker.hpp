#pragma once

#include "visit_storage.hpp"
#include "succ_func.hpp"
#include "checker_setting.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Main class of the computation - responsible for the CMC procedure.
///
/// ModelChecker class solves the parameter analysis problem by iterative transfer of feasible parametrizations from initial states to final ones.
/// Functions in model checker use many supporting variables and therefore are quite long, it would not make sense to split them, though.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModelChecker {
	// Information
	const ProductStructure & product; ///< Product on which the computation will be conducted.

public:
	ModelChecker(const ProductStructure & _product) : product(_product) { }

	/**/
	VisitStorage conductCheck(const CheckerSetting & _settings, const Levels & _parametrization) {
		VisitStorage storage(product.getStateCount());
		vector<StateID> updates = _settings.getInitials(product);
		if (!_settings.circ) {
			for (const StateID ID : updates) {
				storage.update(ID);
			}
		}
		vector<StateID> next_updates; ///< Updates that are sheduled forn the next round.
		bool sat = false;

		// While there are updates, pass them to succesing vertices
		while (!updates.empty())  {
			storage.incCost();
			for (const StateID ID : updates) {
				// Get passed states and remove those already found
				vector<StateID> transports = SuccFunc::broadcastParameters(_parametrization, product, ID);
				auto trans_end = remove_if(WHOLE(transports), [&storage](const StateID t_ID){return storage.isFound(t_ID); });
				
				for (auto next_it = begin(transports); next_it != trans_end; next_it++) {
					storage.update(*next_it);
				}

				next_updates.insert(end(next_updates), begin(transports), trans_end);

				// Don't update when looking for a loop
				if (!(_settings.circ && storage.getCost() == 1)) {
					if (_settings.isFinal(ID, product))
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
};
