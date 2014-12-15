#pragma once

#include "../data/product_structure.hpp"

namespace SuccFunc {
	/**
	 * @return true if this transition is open for given parametrization
	 */
	inline bool isOpen(const Levels & parametrization, const TransConst & trans_cost) {
		if (trans_cost.req_dir)
			return parametrization[trans_cost.param_no] > trans_cost.req_value;
		else
			return parametrization[trans_cost.param_no] < trans_cost.req_value;
	}

	/**
	 * @return vector of reachable targets from ID for this parametrization
	 */
	template <class State>
	vector<StateID> broadcastParameters(const Levels & parametrization, const TSInterface<State> & ts, const StateID ID) {
		// To store parameters that passed the transition but were not yet added to the target
		vector<StateID> param_updates;

		// Cycle through all the transition
		for (size_t trans_num = 0; trans_num < ts.getTransitionCount(ID); trans_num++) {
			StateID target_ID = ts.getTargetID(ID, trans_num);

			// From an update strip all the parameters that can not pass through the transition - color intersection on the transition
			if (SuccFunc::isOpen(parametrization, ts.getTransitionConst(ID, trans_num)))
				param_updates.push_back(target_ID);
		}

		if (param_updates.empty())
			param_updates = ts.getLoops(ID);

		return param_updates;
	}
}