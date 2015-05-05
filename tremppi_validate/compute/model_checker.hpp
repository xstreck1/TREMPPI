#pragma once

#include "visit_storage.hpp"
#include "checker_setting.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Main class of the computation - responsible for the CMC procedure.
///
/// ModelChecker class solves the parameter analysis problem by iterative transfer of feasible parametrizations from initial states to final ones.
/// Functions in model checker use many supporting variables and therefore are quite long, it would not make sense to split them, though.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace ModelChecker {
	/**
	* @return true if this transition is open for given parametrization
	*/
	bool isOpen(const Levels & parametrization, const TransConst & trans_cost);

	/**
	* @return (vector of reachable targets from ID for this parametrization, is the state stable?)
	*/
	template <class State>
	pair<vector<StateID>, bool> broadcastParameters(const Levels & parametrization, const TSInterface<State> & ts, const StateID ID);

	/**
	*
	*/
	VisitStorage conductCheck(const ProductStructure & product, const CheckerSetting & _settings, const Levels & _parametrization, VisitStorage storage);
};

template <class State>
pair<vector<StateID>, bool> ModelChecker::broadcastParameters(const Levels & parametrization, const TSInterface<State> & ts, const StateID ID) {
	vector<StateID> targets;
	bool hell = false; ///< A transition to hell is available

					   // Cycle through all the transition
	for (size_t trans_no = 0; trans_no < ts.getTransitionCount(ID); trans_no++) {
		StateID target_ID = ts.getState(ID)._transitions[trans_no]._t_ID;

		// From an update strip all the parameters that can not pass through the transition - color intersection on the transition
		if (isOpen(parametrization, ts.getState(ID)._transitions[trans_no]._trans_const)) {
			if (target_ID == INF)
				hell = true;
			else
				targets.push_back(target_ID);
		}
	}

	// Add loops only if there is no transition (even a hell transition) available.
	if (!hell && targets.empty())
		return make_pair(ts.getState(ID)._loops, true);
	else
		return make_pair(targets, false);
}