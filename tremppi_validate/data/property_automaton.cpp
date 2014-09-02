#include "property_automaton.hpp"

#include <tremppi_common/network/constraint_parser.hpp>

CompID PropertyHelper::findID(const string & name, const PropertyAutomaton & property) {
	for (const PropertyAutomaton::State & state : property.states)
		if (state.name.compare(name) == 0)
			return state.ID;

	return INF;
}

pair<Levels, Levels> PropertyHelper::getBounds(const RegInfos & reg_infos, const PropertyAutomaton & property) {
	ConstraintParser * cons_pars = new ConstraintParser(reg_infos.size(), DataInfo::getMaxLevel(reg_infos));

	// Impose constraints
	Levels maxes;
	rng::transform(reg_infos, back_inserter(maxes), [](const RegInfo & reg_info){ return reg_info.max_activity; });
	cons_pars->addBoundaries(maxes, true);
	cons_pars->applyFormula(DataInfo::getAllNames(reg_infos), property.experiment);

	// Propagate
	cons_pars->status();

	return pair < Levels, Levels > { cons_pars->getBounds(false), cons_pars->getBounds(true) };
}