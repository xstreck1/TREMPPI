#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include "automaton_interface.hpp"
#include "property_info.hpp"

using MeasurementCons = pair<Configurations, bool>;
using MeasurementsCons = vector<MeasurementCons>;

using DeltasCons = vector<PathCons>;

/// Single labelled transition from one state to another.
struct AutTransitionion : public TransitionProperty 
{
	MeasurementsCons _measurements_cons; // List of reuqirements for state values. Or that are true must be matched, all that are false must not
	DeltasCons _deltas_cons; //< Contraints on trainsitions from this state (ordered by components)

	AutTransitionion(const StateID target_ID, const MeasurementsCons measurements_cons, const DeltasCons deltas_cons);
};
struct AutState : public AutomatonStateProperty<AutTransitionion>
{
	MeasurementsCons _initial_cons;
	MeasurementsCons _accepting_cons;

	AutState(const StateID ID, MeasurementsCons initial_cons, MeasurementsCons accepting_cons);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief A Buchi automaton designed to control some \f$\omega\f$-regular property.
///
/// AutomatonStructure stores Buchi automaton with edges labelled by values the KS can be in for the transition to be allowed.
/// AutomatonStructure data can be set only from the AutomatonStructureBuilder object.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct AutomatonStructure : public AutomatonInterface<AutState> 
{
	//
	GROUNDED(AutomatonStructure);
	//
	static char IDToName(const size_t ID);
	//
	static size_t NameToID(const char name);
};

