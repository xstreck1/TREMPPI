#include "kinetics_translators.hpp"

ParamNo KineticsTranslators::getSpaceSize(const Kinetics & kinetics) {
	return accumulate(begin(kinetics.components), end(kinetics.components), static_cast<ParamNo>(1), [](const ParamNo A, const Kinetics::Component & B) -> ParamNo {
		return A * B.subcolors.size();
	});
}

Levels KineticsTranslators::getSpecieVals(const Kinetics & kinetics, ParamNo param_no) {
	// Prepare storage vector
	Levels specie_vals(kinetics.components.size());
	auto spec_it = specie_vals.begin();

	// Go through colors backwards
	ParamNo divisor = getSpaceSize(kinetics);
	for (auto kin_it = kinetics.components.begin(); kin_it != kinetics.components.end(); kin_it++, spec_it++) {
		// lower divisor value
		divisor /= kin_it->subcolors.size();
		// pick a number for current specie
		*spec_it = static_cast<ActLevel>(param_no / divisor);
		// take the rest for next round
		param_no = param_no % divisor;
	}

	return specie_vals;
}

Levels KineticsTranslators::createParamVector(const Kinetics & kinetics, const ParamNo param_no) {
	Levels result;
	// compute numbers of partial parametrizations for each component
	const Levels color_parts = getSpecieVals(kinetics, param_no);

	for (const CompID ID : cscope(kinetics.components)) 
		result = result + kinetics.components[ID].subcolors[color_parts[ID]];

	// Change the last value
	return result;
}

string KineticsTranslators::createParamString(const Kinetics & kinetics, const ParamNo param_no) {
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

string KineticsTranslators::makeConcise(const Kinetics::Param & param, const string target_name) {
	string context = "K_" + target_name + "_";
	for (auto values : param.requirements)
		context += to_string(values.second.front());
	return context;
}