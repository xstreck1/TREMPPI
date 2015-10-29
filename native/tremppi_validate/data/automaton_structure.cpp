#include "automaton_structure.hpp"

AutTransitionion::AutTransitionion(const StateID t_ID, const MeasurementsCons measurements_cons, const DeltasCons deltas_cons)
	: TransitionProperty(t_ID), _measurements_cons(move(measurements_cons)), _deltas_cons(move(deltas_cons))
{}

AutState::AutState(const StateID ID, const MeasurementsCons initial_cons, const MeasurementsCons accepting_cons)
	: StateProperty(ID), AutomatonStateProperty(ID, true, true), _initial_cons(move(initial_cons)), _accepting_cons(move(accepting_cons))
{}

char AutomatonStructure::IDToName(const size_t ID) 
{
	return static_cast<char>(ID + 65);
}

size_t AutomatonStructure::NameToID(const char name) 
{
	return static_cast<size_t>(name) - 65u;
}

