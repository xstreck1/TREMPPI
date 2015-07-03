#include "analysis_manager.hpp"


tuple<size_t, multimap<StateID, StateID>, double> AnalysisManager::standard(const Levels & parametrization) 
{
	tuple<size_t, multimap<StateID, StateID>, double > result;
	get<2>(result) = 0.0;
	get<0>(result) = INF;

	CheckerSetting settings;
	settings.bfs_bound = bfs_bound - 1; // We need at least one step for the loop on the final state
	settings.bound_type = BoundType::bt_step;
	settings.circ = false;
	settings.stable = false;

	VisitStorage reach_storage(product.size());
	VisitStorage loop_storage(product.size());

	reach_storage = ModelChecker::conductCheck(product, settings, parametrization, move(reach_storage));
	// step 2
	settings.bound_type = BoundType::bt_min;
	settings.circ = true;

	for (const StateID ID : product.getFinalStates()) 
	{
		if (reach_storage.isFound(ID)) {
			settings.initial_states = settings.final_states = { ID };
			settings.bfs_bound = bfs_bound - reach_storage.getVisit(ID);
			loop_storage = ModelChecker::conductCheck(product, settings, parametrization, move(loop_storage));

			if (loop_storage.isFound(ID)) 
			{
				get<0>(result) = std::min(reach_storage.getVisit(ID) + loop_storage.getCost(), get<0>(result));
			}
		}
	}

	// Compute the analysis for the lasso

	if (get<0>(result) != INF && (witness || robustness)) 
	{
		for (const StateID ID : product.getFinalStates()) {

			if (reach_storage.isFound(ID)) 
			{
				// Get the loop visits
				settings.initial_states = settings.final_states = { ID };
				settings.bfs_bound = bfs_bound - reach_storage.getVisit(ID);
				settings.circ = true;
				loop_storage = ModelChecker::conductCheck(product, settings, parametrization, move(loop_storage));


				if (loop_storage.isFound(ID) && reach_storage.getVisit(ID) + loop_storage.getCost() <= get<0>(result)) 
				{
					// Compute reach analysis
					settings.initial_states.clear(); // Reset initals to all
					settings.circ = false;
					auto reach_wit = searcher.findWitnesses(settings, parametrization, reach_storage);
					double reach_rob = robustness ? computer.compute(settings, parametrization, reach_storage, reach_wit) : 0;

					// Compute loop analysis
					settings.initial_states = { ID };
					settings.circ = true;
					auto loop_wit = searcher.findWitnesses(settings, parametrization, loop_storage);
					double loop_rob = robustness ? computer.compute(settings, parametrization, loop_storage, loop_wit) - 1 : 0;

					// Merge
					get<1>(result).insert(WHOLE(reach_wit));
					get<1>(result).insert(WHOLE(loop_wit));
					get<2>(result) += reach_rob * loop_rob;
				}
			}
		}
	}

	return result;
}


tuple<size_t, multimap<StateID, StateID>, double> AnalysisManager::finite(const Levels & parametrization, const bool stable) 
{
	tuple<size_t, multimap<StateID, StateID>, double > result;
	get<2>(result) = 0.0;
	get<0>(result) = INF;

	CheckerSetting settings;
	settings.bfs_bound = bfs_bound;
	settings.bound_type = BoundType::bt_min;
	settings.circ = false;
	settings.stable = stable;

	VisitStorage storage(product.size());
	storage = ModelChecker::conductCheck(product, settings, parametrization, move(storage));


	if (storage.succeeded() && (witness || robustness)) 
	{
		auto transitions = searcher.findWitnesses(settings, parametrization, storage);
		if (witness)
			get<1>(result) = transitions;

		get<2>(result) = robustness ? computer.compute(settings, parametrization, storage, transitions) : 0;
	}

	get<0>(result) = storage.getCost();
	return result;
}

AnalysisManager::AnalysisManager(const ProductStructure & _product, const size_t _bfs_bound, const bool _witness, const bool _robustness) :
	product(_product),
	searcher(WitnessSearcher(_product)),
	computer(RobustnessCompute(_product)),
	bfs_bound(_bfs_bound),
	witness(_witness),

	robustness(_robustness) 
	{
}


tuple<size_t, multimap<StateID, StateID>, double> AnalysisManager::check(const Levels & parametrization) 
{
	switch (product._aut_type) {
	case BA_standard:
		return standard(parametrization);
	case BA_stable:
		return finite(parametrization, true);
	case BA_finite:
		return finite(parametrization, false);
	};
}
