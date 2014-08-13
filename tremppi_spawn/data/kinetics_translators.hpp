#pragma once

#include "kinetics.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Functions that take take in the Kinetics data structure and compute new data from them.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace KineticsTranslators {
	// @return the number of parametrizations
	inline ParamNo getSpaceSize(const Kinetics & kinetics) {
		return accumulate(begin(kinetics.components), end(kinetics.components), static_cast<ParamNo>(1), [](const ParamNo A, const Kinetics::Component & B) -> ParamNo {
			return A * B.col_count;
		});
	}

	// @return 
	Levels getSpecieVals(const Kinetics & kinetics, ParamNo param_no) {
		// Prepare storage vector
		Levels specie_vals(kinetics.components.size());
		auto spec_it = specie_vals.begin();

		// Go through colors backwards
		ParamNo divisor = getSpaceSize(kinetics);
		for (auto kin_it = kinetics.components.begin(); kin_it != kinetics.components.end(); kin_it++, spec_it++) {
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

		for (const CompID ID : cscope(kinetics.components)) {
			for (auto & param : kinetics.components[ID].params) {
				result.emplace_back(param.target_in_subcolor[color_parts[ID]]);
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


		// Test subparametrizations for all components.
		size_t begin = 0;
		for (const CompID ID : cscope(kinetics.components)) {
			vector<ParamNo> submatch;
			auto & params = kinetics.components[ID].params;

			// Try to match all the subcolors for the current specie
			for (const size_t subolor_no : crange(kinetics.components[ID].col_count)) {
				bool valid = true;
				for (const size_t param_no : cscope(params)) {
					if (param_vals[param_no + begin] != -1 && param_vals[param_no + begin] != params[param_no].target_in_subcolor[subolor_no]) {
						valid = false;
						break;
					}
				}
				if (valid) submatch.push_back(subolor_no * kinetics.components[ID].step_size);
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
}