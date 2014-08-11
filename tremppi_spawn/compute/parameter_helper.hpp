/* Copyright (C) 2012-2013 - Adam Streck
 * This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
 * ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
 * ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
 */

#pragma once

#include "../kinetics/kinetics.hpp"
#include "../model/model_helper.hpp"
#include "../model/property_automaton.hpp"

namespace ParameterHelper {

	static void find_functional(const Model &model, const PropertyAutomaton & property, Kinetics & kinetics) {
		pair<Levels, Levels> bounds = ModelHelper::getBounds(model, property);

		for (auto & specie : kinetics.species) {
			for (auto & param : specie.params) {
				for (auto & req : param.requirements) {
					// If lower bound is higher than max possible specie value or the upper bound is lower than min possible specie value.
					if ((bounds.first[req.first] > req.second.back()) || (bounds.second[req.first] < req.second.front())){
						param.functional = false;
					}
				}
			}
		}
	}
};