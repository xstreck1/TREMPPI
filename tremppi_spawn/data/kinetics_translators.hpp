#pragma once

#include "kinetics.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Functions that take take in the Kinetics data structure and compute new data from them.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace KineticsTranslators {
	// @return the number of parametrizations
	inline ParamNo getSpaceSize(const Kinetics & kinetics);

	// @return parametrization vector from its number
	Levels createParamVector(const Kinetics & kinetics, const ParamNo param_no);

	// @return 
	Levels getSpecieVals(const Kinetics & kinetics, ParamNo param_no);

	// @return parametrizations string in the form "(val_1,...,val_n)".
	string createParamString(const Kinetics & kinetics, const ParamNo param_no);

	// @return representation of the parametrization used by the database
	string makeConcise(const Kinetics::Param & param, const string target_name);
}