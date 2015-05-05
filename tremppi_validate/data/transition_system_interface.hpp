#pragma once

#include "graph_interface.hpp"

/// Structure with constraints on a transition within a TS
struct TransConst {
	ParamNo _param_no : 61; ///< id of the parameter
	bool _req_dir : 1; ///< true for increase, false for decrease
	ActLevel _req_value : 4; ///< value of the specie that's being questioned
};

/// Storing a single transition to neighbour state together with its transition function.
struct TSTransitionProperty : public TransitionProperty {
	TransConst _trans_const;

	inline TSTransitionProperty(const StateID t_ID, TransConst trans_const)
		: TransitionProperty(t_ID), _trans_const(trans_const) {}
};

/// State having specie levels attached.
template <typename Transition>
struct TSStateProperty : public virtual StateProperty<Transition> {
	Levels _levels; ///< Species_level[i] = activation level of specie i.
	vector<StateID> _loops; ///< States with the Same KS ID, but different BA that are possible targets

	inline TSStateProperty(const StateID ID, const Levels& levels)
		: StateProperty<Transition>(ID), _levels(levels) { } ///< Simple filler, assigns values to all the variables.
};

/// Transition system has states labeled with activity levels and transitions labeleld with constraints.
template<typename StateT>
class TSInterface : public virtual GraphInterface<StateT> {
public:
	inline void addLoop(const StateID s_ID, const StateID t_ID) {
		_states[s_ID]._loops.emplace_back(t_ID);
	}
};
