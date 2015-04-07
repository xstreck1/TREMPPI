#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/constraint_parser.hpp>
#include "automaton_interface.hpp"

/// Single labelled transition from one state to another.
struct AutTransitionion : public TransitionProperty {
	mutable ConstraintParser * trans_constr; ///< Allowed values of species for this transition.

	// Move constructor is necessary as the parent (Automaton) is passed around by moving. The memory must be kept safe.
	AutTransitionion(AutTransitionion && other);
	AutTransitionion& operator=(AutTransitionion && other);
	AutTransitionion(const StateID target_ID, ConstraintParser * _trans_constr);
	AutTransitionion(const AutTransitionion &) = delete;
	AutTransitionion& operator=(const AutTransitionion &) = delete;
	~AutTransitionion();
};

/// Storing a single state of the Buchi automaton. This state is extended with a value saying wheter the states is final.
struct AutState : public AutomatonStateProperty<AutTransitionion> {
	vector<CompID> stables; //< IDs of the components that must not change before the next state is reached (so not even when transiting into the next state)

	/// Fills data and checks if the state has value  -> is initial
	AutState(const StateID ID, const bool final, const vector<StateID> _stables);
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
	AutomatonStructure();
	AutomatonStructure(AutomatonStructure && other);
	AutomatonStructure(const AutomatonStructure &) = delete;
	AutomatonStructure& operator=(const AutomatonStructure &) = delete;
	AutomatonStructure& operator= (AutomatonStructure && other);
	~AutomatonStructure();

	ConstraintParser * init_constr;
	ConstraintParser * acc_constr;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// FILLING METHODS (can be used only from AutomatonStructureBuilder)
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 * @param final	if true than state with index equal to the one of this vector is final
	 */
	void addState(const StateID ID, const bool final, const vector<StateID> stables);


	//
	void addTransition(const StateID ID, AutTransitionion transition);

	// Gecode accepts only a raw pointer for the searcher.
	ConstraintParser * getTransitionConstraint(const StateID ID, const size_t trans_no) const;

	// @return	the stables for the given state
	const vector<CompID> & getStables(const StateID ID) const;
};

