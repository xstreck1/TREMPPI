#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/constraint_parser.hpp>
#include "automaton_interface.hpp"
#include "property_info.hpp"

/// Single labelled transition from one state to another.
struct AutTransitionion : public TransitionProperty {
	Configurations _state_constr; //< Boundary on the state values in the current configuraion
	vector<PathCons> _path_constr; //< Contraints on trainsitions from this state (ordered by components)

	// Move constructor is necessary as the parent (Automaton) is passed around by moving. The memory must be kept safe.
	AutTransitionion(const StateID target_ID, const Configurations & state_constr, const vector<PathCons> & path_constr);
	NO_COPY_SHORT(AutTransitionion);
	DEFAULT_MOVE(AutTransitionion);
};

/// Storing a single state of the Buchi automaton. This state is extended with a value saying wheter the states is final.
struct AutState : public AutomatonStateProperty<AutTransitionion> {
	AutState(const StateID ID, const bool is_initial, const bool is_final);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief A Buchi automaton designed to control some \f$\omega\f$-regular property.
///
/// AutomatonStructure stores Buchi automaton with edges labelled by values the KS can be in for the transition to be allowed.
/// AutomatonStructure data can be set only from the AutomatonStructureBuilder object.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AutomatonStructure : public AutomatonInterface<AutState> {
protected:
	Configurations _init_constr;
	Configurations _acc_constr;

public:
	AutomatonStructure(const AutType aut_type, const Configurations & init_const, const Configurations & acc_constr);
	NO_COPY_SHORT(AutomatonStructure);
	DEFAULT_MOVE(AutomatonStructure);
	
	// 
	void addState(const StateID ID, const bool is_initial, const bool is_final);

	//
	void addTransition(const StateID ID, const StateID t_ID, const Configurations & state_constr, const vector<PathCons> & path_constr)

	// 
	const Configurations & getStateConstr(const StateID ID, const size_t trans_no) const;

	// 
	const vector<PathCons>& getPathConstr(const StateID ID, const size_t trans_no) const;

	//
	const Configurations & getInitConstr() const;

	//
	const Configurations & getAccConstr() const;
};

