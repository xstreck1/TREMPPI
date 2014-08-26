#pragma once

#include <tremppi_common/network/kinetics_translators.hpp>
#include <tremppi_common/network/model_translators.hpp>

#include "../model/property_automaton.hpp"

namespace ParameterHelper {
	// @bounds on component values after propagatin the experiment
	pair<Levels, Levels> getBounds(const Model & model, const PropertyAutomaton & property) {
		ConstraintParser * cons_pars = new ConstraintParser(model.components.size(), ModelTranslators::getMaxLevel(model));

		// Impose constraints
		Levels maxes;
		rng::transform(model.components, back_inserter(maxes), [](const Model::ModelComp & specie){ return specie.max_activity; });
		cons_pars->addBoundaries(maxes, true);
		cons_pars->applyFormula(ModelTranslators::getAllNames(model), property.getExperiment());

		// Propagate
		cons_pars->status();

		return pair<Levels, Levels>{ cons_pars->getBounds(false), cons_pars->getBounds(true) };
	}
};