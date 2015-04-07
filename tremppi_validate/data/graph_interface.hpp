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
	const StateID target_ID;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief This is just a very simple basis for a state of any graph.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Transition>
struct StateProperty {
	/// Unique ID of the state.
	const StateID ID;
	/// Graph or automaton transitions, basically it is an edge with a label.
	vector<Transition> transitions;

	/**
	 * Basic constructor that fills in the ID and label.
	 */
	StateProperty<Transition>(const StateID _ID) : ID(_ID) { }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Interface for all the classes that represent a directed graph. Transitions are expected to be stored within their source state structure.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename StateT>
class GraphInterface {
protected:
	vector<StateT> states; ///< Vector holding states of the graph.

public:
	NO_COPY(GraphInterface)
		virtual ~GraphInterface() = default;

	/**
	 * Obtains number of states of the graph.
	 * @return integer with size of the graph
	 */
	inline size_t getStateCount() const {
		return states.size();
	}

	/**
	 * Obtains number of outcoming transitions for given state.
	 * @param ID	ID of the state to get the number from
	 * @return	integer with number of outcoming transitions
	 */
	inline size_t getTransitionCount(const StateID ID) const {
		return states[ID].transitions.size();
	}

	/**
	 * Obtains ID of the target of given transition for given state.
	 * @param ID	ID of the state to get the neighbour from
	 * @param trans_number	index in the vector of transitions
	 * @return	ID of the requested target
	 */
	inline StateID getTargetID(const StateID ID, const size_t transition_number) const {
		return states[ID].transitions[transition_number].target_ID;
	}

	/**
	 * Returns given state as a string.
	 * @param ID	ID of the state to turn into the string
	 * @return	given state as a string
	 */

	const string getString(const StateID ID) const {
		return to_string(ID);
	}
};