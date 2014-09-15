#pragma once
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/data_info.hpp>

struct PropertyAutomaton {
	NO_COPY(PropertyAutomaton)

		/// Edge in Buchi Automaton
	struct Edge {
		StateID target_ID;
		string constraint;
	};
	typedef vector<Edge> Edges; ///< Set of outgoing edges.

	struct State {
		string name; ///< Label of the state.
		CompID ID; ///< Numerical constant used to distinguish the state. Starts from 0!
		bool final; ///< True if the state is final.

		Edges edges; ///< Edges in Buchi Automaton (Target ID, edge label).
	};

	string name;
	string prop_type; ///< What property does this automaton hold.
	vector<State> states; ///< vector of all states of the controlling Buchi automaton
	string experiment;
};

namespace PropertyHelper {
	/**
	 * Finds ordinal number of the BA state based on its name or number string.
	 * @return	number of the state with the specified name if there is such, otherwise INF
	 */
	CompID findID(const string & name, const PropertyAutomaton & property);

	// @bounds on component values after propagatin the experiment
	pair<Levels, Levels> getBounds(const RegInfos & reg_infos, const PropertyAutomaton & property);
}
