#include "checker_setting.hpp"

CheckerSetting::CheckerSetting() : bound_type(BoundType::bt_inf), bfs_bound(INF), circ(false), stable(false), minimal_count(1) { }

bool CheckerSetting::isInitial(const StateID ID, const ProductStructure & product) const {
	if (initial_states.empty())
		return product._states[ID]._initial;
	else
		return (find(initial_states.begin(), initial_states.end(), ID) != initial_states.end());
}

bool CheckerSetting::isFinal(const StateID ID, const ProductStructure & product) const {
	if (final_states.empty())
		return product._states[ID]._final;
	else
		return (find(final_states.begin(), final_states.end(), ID) != final_states.end());
}

const vector<StateID> CheckerSetting::getInitials(const ProductStructure & product) const {
	if (initial_states.empty())
		return product.getInitialStates();
	else
		return initial_states;
}

const vector<StateID> CheckerSetting::getFinals(const ProductStructure & product) const {
	if (final_states.empty())
		return product.getFinalStates();
	else
		return final_states;
}
