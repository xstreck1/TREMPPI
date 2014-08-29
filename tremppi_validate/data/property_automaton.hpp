/*
 * Copyright (C) 2012-2013 - Adam Streck
 * This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
 * ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
 * ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
 */

#ifndef PROPERTY_AUTOMATON_HPP
#define PROPERTY_AUTOMATON_HPP

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/definitions.hpp>

class PropertyAutomaton {
public:
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

	/**
	 * Finds ordinal number of the BA state based on its name or number string.
	 * @return	number of the state with the specified name if there is such, otherwise INF
	 */
	CompID findID(const string & name) const {
		for (const State & state : states)
		if (state.name.compare(name) == 0)
			return state.ID;

		return INF;
	}
};

#endif // PROPERTY_AUTOMATON_HPP
