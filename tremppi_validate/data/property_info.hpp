#pragma once
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/data_info.hpp>

enum BoundType { bt_inf, bt_min, bt_step }; //< What sort of bound the user placed - do the full check / check until the minimal lenght is found for a parametrization / use only bounded amount of steps
enum PathCons { pc_none, pc_up, pc_down, pc_stay }; ///< What are the requirements on the transitions between measurements? (none, monotonicity, stability)

struct PropertyInfo {
	NO_COPY(PropertyInfo)

	struct State {
		CompID ID; ///< Numerical constant used to distinguish the state. Starts from 0!
		map<string, ActRange> state_constraints; ///< Per component state constraints
		map<string, PathCons> path_constraints; ///< Per component path constraints
	};
	vector<State> states; ///< vector of all states of the controlling Buchi automaton

	string name; // Identifier for the property
	string ending; // What should happen after last measurement? (anything, stability is reached, there is return to another) 
	map<string, ActRange> bounds; // Per component Range of component activity (bound inclusive)
	// Execution constraints
	size_t bound; // Maximal search depth...
	bool robustness; // Should compute robustness?
	bool witness; // Should compute witness?
};

