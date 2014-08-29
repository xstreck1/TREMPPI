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
	struct Constraints {
		string values;
		bool transient;
		bool stable;

		Constraints(const string & values_, const bool transient_, const bool stable_) : values(values_), transient(transient_), stable(stable_) {}
		Constraints(const string & values_) : Constraints(values_, false, false) {}
		Constraints() : Constraints("") {}
	};

	/// Edge in Buchi Automaton
	struct Edge {
		StateID target_ID;
		Constraints cons;
	};
	typedef vector<Edge> Edges; ///< Set of outgoing edges.

	pair<size_t, size_t> accepting_range; ///< The minimal and the maximal number of accepting states in the model checking

private:
	struct AutomatonState {
		string name; ///< Label of the state.
		CompID ID; ///< Numerical constant used to distinguish the state. Starts from 0!
		bool final; ///< True if the state is final.

		Edges edges; ///< Edges in Buchi Automaton (Target ID, edge label).
	};

	PropType prop_type; ///< What property does this automaton hold.
	vector<AutomatonState> states; ///< vector of all states of the controlling Buchi automaton

public:
	string experiment;
	size_t min_acc; ///< Minimal number of accepting states required for acceptation (LTL counting)
	size_t max_acc; ///< Maximal number --||--

	PropertyAutomaton(const PropType _prop_type = LTL) : prop_type(_prop_type), experiment("tt"), min_acc(1u), max_acc(INF) { }

	/**
	 * Add a new state to the automaton. If the name is empty, then "ID of the automaton" + "letter" is used.
	 *
	 * @return	ID of state in the vector
	 */
	inline size_t addState(string name, bool final) {
		if (name.empty())
			name = to_string(states.size());
		states.push_back({ name, states.size(), final, Edges() });
		return states.size() - 1;
	}

	/**
	 * @return	number of the states
	 */
	inline size_t getStatesCount() const {
		return states.size();
	}

	/**
	 * Add a new edge - edge is specified by the target state and label.
	 */
	inline void addEdge(const StateID source_ID, StateID target_ID, const Constraints cons) {
		states[source_ID].edges.push_back({ target_ID, cons });
	}

	/**
	 * Finds ordinal number of the BA state based on its name or number string.
	 * @return	number of the state with the specified name if there is such, otherwise INF
	 */
	CompID findID(const string & name) const {
		for (const AutomatonState & state : states)
		if (state.name.compare(name) == 0)
			return state.ID;

		return INF;
	}

	const string & getName(StateID ID) const {
		return states[ID].name;
	}

	inline bool isFinal(const size_t ID) const {
		return states[ID].final;
	}

	inline const Edges & getEdges(const CompID ID) const {
		return states[ID].edges;
	}

	inline PropType getPropType() const {
		return prop_type;
	}

	inline size_t getMinAcc() const {
		return min_acc;
	}

	inline size_t getMaxAcc() const {
		return max_acc;
	}

	inline const string & getExperiment() const {
		return experiment;
	}

	inline bool isCountingUsed() const {
		return (min_acc != 1) || (max_acc != INF);
	}

	static bool isCountingUsed(const size_t min_acc, const size_t max_acc) {
		return (min_acc != 1) || (max_acc != INF);
	}
};

#endif // PROPERTY_AUTOMATON_HPP
