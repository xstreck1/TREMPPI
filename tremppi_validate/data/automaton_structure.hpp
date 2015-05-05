#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include "automaton_interface.hpp"
#include "property_info.hpp"

/// Single labelled transition from one state to another.
struct AutTransitionion : public TransitionProperty {
	Configurations _state_constr; //< Boundary on the state values in the current configuraion
	vector<PathCons> _path_constr; //< Contraints on trainsitions from this state (ordered by components)

	AutTransitionion(const StateID target_ID, const Configurations & state_constr, const vector<PathCons> & path_constr);
};

using AutState = AutomatonStateProperty<AutTransitionion>;

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
	GROUNDED(AutomatonStructure);

	//
	void setInitContrs(const Configurations & init_constr);

	//
	void setAccContrs(const Configurations & acc_constr);
	
	// 
	const Configurations & getStateConstr(const StateID ID, const size_t trans_no) const;

	// 
	const vector<PathCons>& getPathConstr(const StateID ID, const size_t trans_no) const;

	//
	const Configurations & getInitConstr() const;

	//
	const Configurations & getAccConstr() const;
};

