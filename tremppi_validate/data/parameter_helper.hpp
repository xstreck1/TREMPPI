#pragma once

#include <tremppi_common/network/kinetics_translators.hpp>
#include <tremppi_common/network/data_info.hpp>

#include "../data/property_automaton.hpp"

namespace ParameterHelper {
	// @bounds on component values after propagatin the experiment
	pair<Levels, Levels> getBounds(const RegInfos & reg_infos, const PropertyAutomaton & property) {
		ConstraintParser * cons_pars = new ConstraintParser(reg_infos.size(), DataInfo::getMaxLevel(reg_infos));

		// Impose constraints
		Levels maxes;
		rng::transform(reg_infos, back_inserter(maxes), [](const RegInfo & reg_info){ return reg_info.max_activity; });
		cons_pars->addBoundaries(maxes, true);
		cons_pars->applyFormula(DataInfo::getAllNames(reg_infos), property.getExperiment());

		// Propagate
		cons_pars->status();

		return pair<Levels, Levels>{ cons_pars->getBounds(false), cons_pars->getBounds(true) };
	}
};