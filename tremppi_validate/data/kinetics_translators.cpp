#include "kinetics_translators.hpp"

inline ParamNo MyKineticsTranslators::getSpaceSize(const MyKinetics & kinetics) {
	return accumulate(begin(kinetics.components), end(kinetics.components), static_cast<ParamNo>(1), [](const ParamNo A, const MyKinetics::Component & B) -> ParamNo {
		return A * B.col_count;
	});
}

Levels MyKineticsTranslators::getSpecieVals(const MyKinetics & kinetics, ParamNo param_no) {
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

Levels MyKineticsTranslators::createParamVector(const MyKinetics & kinetics, const ParamNo param_no) {
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

string MyKineticsTranslators::createParamString(const MyKinetics & kinetics, const ParamNo param_no) {
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

string MyKineticsTranslators::makeConcise(const MyKinetics::Param & param, const string target_name) {
	string context = "K_" + target_name + "_";
	for (auto values : param.requirements)
		context += to_string(values.second.front());
	return context;
}