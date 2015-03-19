#pragma once

#include "../data/product_structure.hpp"

// #define DETERMINISM

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
		vector<StateID> targets;
		bool hell = false; ///< A transition to hell is available

		// Cycle through all the transition
		for (size_t trans_num = 0; trans_num < ts.getTransitionCount(ID); trans_num++) {
			StateID target_ID = ts.getTargetID(ID, trans_num);

			// From an update strip all the parameters that can not pass through the transition - color intersection on the transition
			if (SuccFunc::isOpen(parametrization, ts.getTransitionConst(ID, trans_num))) {
				if (target_ID == INF)
					hell = true;
				else
					targets.push_back(target_ID);
			}
		}

#ifdef DETERMINISM
		if (targets.size() != 1)
			targets.clear();
#else
		// Add loops only if there is no transition (even a hell transition) available.
		if (!hell && targets.empty())
			targets = ts.getLoops(ID);
#endif

		return targets;
	}
}