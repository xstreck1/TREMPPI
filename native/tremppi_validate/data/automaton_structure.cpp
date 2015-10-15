#include "automaton_structure.hpp"

AutTransitionion::AutTransitionion(const StateID t_ID, const std::vector<std::pair<Configurations, bool> > state_constr,  const vector<PathCons> path_constr)
	: TransitionProperty(t_ID), _state_constr(move(state_constr)), _path_constr(move(path_constr)) {}



char AutomatonStructure::IDToName(const size_t ID) 
{
	return static_cast<char>(ID + 65);
}


size_t AutomatonStructure::NameToID(const char name) 
{
	return static_cast<size_t>(name) - 65u;
}
