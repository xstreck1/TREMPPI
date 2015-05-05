#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/definitions.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file This file holds abstract basis for a graph with states and transitions between the states.
/// Graph is parametrized by state and state is parametrized by transition. Basic state and transition are also defined here.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief This is just a very simple basis for a transition in a graph.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct TransitionProperty {
	/// Unique ID of the state.
	const StateID _t_ID;

	TransitionProperty(const StateID t_ID)
		: _t_ID(t_ID) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief This is just a very simple basis for a state of any graph.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Transition>
struct StateProperty {
	/// Unique ID of the state.
	const StateID _ID;
	/// Graph or automaton transitions, basically it is an edge with a label.
	vector<Transition> _transitions;

	StateProperty(const StateID ID) 
		: _ID(ID) {}

	size_t size() const {
		return _transitions.size();
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Interface for all the classes that represent a directed graph. Transitions are expected to be stored within their source state structure.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename StateT, typename TransitionT = decltype(declval<StateT>()._transitions.front())>
struct GraphInterface {
protected:
	vector<StateT> _states; ///< Vector holding states of the graph.

public:
	NO_COPY(GraphInterface)

	/**
	 *
	 */
	void addState(StateT state) {
		_states.emplace_back(move(state));
	}

	/**
	 *
	 */
	void addTransition(const CompID s_ID, typename TransitionT transition) {
		_states[s_ID]._transitions.emplace_back(move(transition));
	}

	/**
	 * Obtains number of states of the graph.
	 * @return integer with size of the graph
	 */
	inline size_t size() const {
		return _states.size();
	}

	/**
	 * Obtains number of outcoming transitions for given state.
	 * @param ID	ID of the state to get the number from
	 * @return	integer with number of outcoming transitions
	 */
	inline size_t getTransitionCount(const StateID ID) const {
		return _states[ID]._transitions.size();
	}

	/**
	 *
	 */
	inline const StateT & getState(const StateID ID) const {
		return _states[ID];
	}

	/**
	 * Returns given state as a string.
	 * @param ID	ID of the state to turn into the string
	 * @return	given state as a string
	 */
	const inline string getString(const StateID ID) const {
		return to_string(ID);
	}
};