#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/constraint_parser.hpp>
#include "automaton_interface.hpp"

/// Single labelled transition from one state to another.
struct AutTransitionion : public TransitionProperty {
	mutable ConstraintParser * trans_constr; ///< Allowed values of species for this transition.

	~AutTransitionion() {
		if (trans_constr != nullptr)
			delete trans_constr;
	}
	// Move constructor is necessary as the parent (Automaton) is passed around by moving. The memory must be kept safe.
	AutTransitionion(AutTransitionion && other) : TransitionProperty(other.target_ID) {
		trans_constr = other.trans_constr;
		other.trans_constr = nullptr;
	}
	AutTransitionion& operator=(AutTransitionion &&) = delete;
	AutTransitionion(const AutTransitionion &) = delete;
	AutTransitionion& operator=(const AutTransitionion &) = delete;

	AutTransitionion(const StateID target_ID, ConstraintParser * _trans_constr)
		: TransitionProperty(target_ID), trans_constr(_trans_constr) {}
};

/// Storing a single state of the Buchi automaton. This state is extended with a value saying wheter the states is final.
struct AutState : public AutomatonStateProperty<AutTransitionion> {

	/// Fills data and checks if the state has value  -> is initial
	AutState(const StateID ID, const bool final)
	: AutomatonStateProperty<AutTransitionion>((ID == 0), final, ID) { }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief A Buchi automaton designed to control some \f$\omega\f$-regular property.
///
/// AutomatonStructure stores Buchi automaton with edges labelled by values the KS can be in for the transition to be allowed.
/// AutomatonStructure data can be set only from the AutomatonStructureBuilder object.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AutomatonStructure : public AutomatonInterface<AutState> {
public:
	friend class AutomatonBuilder;
	AutomatonStructure() = default;
	AutomatonStructure(AutomatonStructure &&) = default;
	AutomatonStructure(const AutomatonStructure &) = delete;
	AutomatonStructure& operator=(const AutomatonStructure &) = delete;
	AutomatonStructure& operator= (AutomatonStructure && other) {
		states = move(other.states);
		my_type = other.my_type;
		initial_states = move(other.initial_states);
		final_states = move(other.final_states);
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// FILLING METHODS (can be used only from AutomatonStructureBuilder)
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 * @param final	if true than state with index equal to the one of this vector is final
	 */
	void addState(const StateID ID, const bool final) {
		states.push_back({ ID, final });
		if (ID == 0)
			initial_states.push_back(ID);
		if (final)
			final_states.push_back(ID);
	}

	//
	void addTransition(const StateID ID, AutTransitionion transition) {
		states[ID].transitions.push_back(move(transition));
	}

	// Gecode accepts only a raw pointer for the searcher.
	ConstraintParser * getTransitionConstraint(const StateID ID, const size_t trans_no) const {
		return states[ID].transitions[trans_no].trans_constr;
	}
};

