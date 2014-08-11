/*
* Copyright (C) 2012-2013 - Adam Streck
* This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
* ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
* ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
* For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
*/

#pragma once

#include "../model/model.hpp"
#include "../kinetics/kinetics.hpp"

namespace KineticsTranslators {
	// @return the number of parametrizations
	inline ParamNo getSpaceSize(const Kinetics & kinetics) {
		return accumulate(begin(kinetics.species), end(kinetics.species), static_cast<ParamNo>(1), [](const ParamNo A, const Kinetics::Specie & B) -> ParamNo {
			return A * B.col_count;
		});
	}

	// @return 
	Levels getSpecieVals(const Kinetics & kinetics, ParamNo param_no) {
		// Prepare storage vector
		Levels specie_vals(kinetics.species.size());
		auto spec_it = specie_vals.begin();

		// Go through colors backwards
		ParamNo divisor = getSpaceSize(kinetics);
		for (auto kin_it = kinetics.species.begin(); kin_it != kinetics.species.end(); kin_it++, spec_it++) {
			// lower divisor value
			divisor /= kin_it->col_count;
			// pick a number for current specie
			*spec_it = static_cast<ActLevel>(param_no / divisor);
			// take the rest for next round
			param_no = param_no % divisor;
		}

		return specie_vals;
	}

	// @return parametrization vector from its number
	Levels createParamVector(const Kinetics & kinetics, const ParamNo param_no) {
		Levels result;
		// compute numbers of partial parametrizations for each component
		const Levels color_parts = getSpecieVals(kinetics, param_no);

		for (const SpecieID ID : cscope(kinetics.species)) {
			for (auto & param : kinetics.species[ID].params) {
				// There may be more contexts than values due to the fact that some are not functional. These are assigned the value -1.
				if (param.functional)
					result.emplace_back(param.target_in_subcolor[color_parts[ID]]);
				else
					result.emplace_back(-1);
			}
		}

		// Change the last value
		return result;
	}

	// @return parametrizations string in the form "(val_1,...,val_n)".
	string createParamString(const Kinetics & kinetics, const ParamNo param_no) {
		string result = "(";
		// Compute numbers of partial parametrizations for each component.
		Levels params = createParamVector(kinetics, param_no);
		// Convert.
		for (const ActLevel param : params)
			result += to_string(param) + ",";
		// Change the last value.
		result[result.size() - 1] = ')';
		return result;
	}

	// @return representation of the parametrization used by the database
	string makeConcise(const Kinetics::Param & param, const string target_name) {
		string context = "K_" + target_name + "_";
		for (auto values : param.requirements)
			context += to_string(values.second.front());
		return context;
	}

	// @Obtain all the parametrizations ID for the given levels string
	set<ParamNo> findMatching(const Kinetics & kinetics, const Levels & param_vals) {
		set<ParamNo> result;


		// Test subparametrizations for all species.
		size_t begin = 0;
		for (const SpecieID ID : cscope(kinetics.species)) {
			vector<ParamNo> submatch;
			auto & params = kinetics.species[ID].params;

			// Try to match all the subcolors for the current specie
			for (const size_t subolor_no : crange(kinetics.species[ID].col_count)) {
				bool valid = true;
				for (const size_t param_no : cscope(params)) {
					if (param_vals[param_no + begin] != -1 && param_vals[param_no + begin] != params[param_no].target_in_subcolor[subolor_no]) {
						valid = false;
						break;
					}
				}
				if (valid) submatch.push_back(subolor_no * kinetics.species[ID].step_size);
			}

			// At least one subparametrization must be found for each specie, if not end.
			if (submatch.empty())
				return set<ParamNo>();

			// Create the results as all possible combinations.
			set<ParamNo> old = result.empty() ? set<ParamNo>({ 0 }) : move(result);
			for (const ParamNo o_match : old) {
				for (const ParamNo m_match : submatch) {
					result.insert(o_match + m_match);
				}
			}

			begin += params.size();
		}
		return result;
	}


	/**
	* @return  the parameter that has the given context

	const Kinetics::Param & matchContext(const Model & model, const Kinetics & kinetics, const string & context, const SpecieID t_ID) {
	const string canonic = ModelTranslators::makeCanonic(model, context, t_ID);
	for (auto & param : kinetics.species[t_ID].params)
	if (param.context.compare(canonic) == 0)
	return param;
	throw runtime_error("Failed to match the context " + context + " for the specie " + to_string(t_ID));
	}*/
}