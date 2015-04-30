#pragma once
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/data_info.hpp>

enum BoundType { inf, min, step }; //< What sort of bound the user placed - do the full check / check until the minimal lenght is found for a parametrization / use only bounded amount of steps
enum PathCons { none, up, down, stay }; ///< What are the requirements on the transitions between measurements? (none, monotonicity, stability)

struct PropertyInfo {
	NO_COPY(PropertyInfo)

	struct State {
		CompID ID; ///< Numerical constant used to distinguish the state. Starts from 0!
		map<string, ActRange> state_constraints; ///< Per component state constraints
		map<string, PathCons> path_constraints; ///< Per component path constraints
	};
	vector<State> states; ///< vector of all states of the controlling Buchi automaton

	map<string, ActRange> bounds;
	string name;
	string prop_type;
	size_t bound;
	bool robustness;
	bool witness;
};

