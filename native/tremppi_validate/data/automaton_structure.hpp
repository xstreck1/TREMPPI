#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include "automaton_interface.hpp"
#include "property_info.hpp"

/// Single labelled transition from one state to another.

struct AutTransitionion : public TransitionProperty 
{
	Configurations _state_constr; //< Boundary on the state values in the current configuraion
	bool _inclusive; //< If true, only the states that match the condition are allows, if false, only those that do not
	vector<PathCons> _path_constr; //< Contraints on trainsitions from this state (ordered by components)

	AutTransitionion(const StateID target_ID, const Configurations & state_constr, const bool inclusive, const vector<PathCons> & path_constr);
};

using AutState = AutomatonStateProperty<AutTransitionion>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief A Buchi automaton designed to control some \f$\omega\f$-regular property.
///
/// AutomatonStructure stores Buchi automaton with edges labelled by values the KS can be in for the transition to be allowed.
/// AutomatonStructure data can be set only from the AutomatonStructureBuilder object.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct AutomatonStructure : public AutomatonInterface<AutState> 
{
	Configurations _init_constr;
	Configurations _acc_constr;

	//
	GROUNDED(AutomatonStructure);

	//
	static char IDToName(const size_t ID);

	//
	static size_t NameToID(const char name);
};

