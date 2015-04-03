#include "property_automaton.hpp"

#include <tremppi_common/network/constraint_parser.hpp>

CompID PropertyHelper::findID(const string & name, const PropertyAutomaton & property_automaton) {
	for (const PropertyAutomaton::State & state : property_automaton.states)
		if (state.name.compare(name) == 0)
			return state.ID;

	return INF;
}

pair<Levels, Levels> PropertyHelper::getBounds(const RegInfos & reg_infos, const PropertyAutomaton & property_automaton) {
	// Impose constraints
	pair<Levels, Levels> result = { Levels(reg_infos.size(), INF), Levels(reg_infos.size(), 0) };
	transform(begin(property_automaton.bounds), begin(property_automaton.bounds) + reg_infos.size(), begin(result.first), [](const pair<ActLevel, ActLevel> & bound) {return get<0>(bound); });
	transform(begin(property_automaton.bounds), begin(property_automaton.bounds) + reg_infos.size(), begin(result.second), [](const pair<ActLevel, ActLevel> & bound) {return get<1>(bound); });
	return result;
}