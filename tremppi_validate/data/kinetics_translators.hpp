#pragma once

#include "kinetics.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Functions that take take in the MyKinetics data structure and compute new data from them.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace MyKineticsTranslators {
	// @return the number of parametrizations
	inline ParamNo getSpaceSize(const MyKinetics & kinetics);

	// @return 
	Levels getSpecieVals(const MyKinetics & kinetics, ParamNo param_no);

	// @return parametrization vector from its number
	Levels createParamVector(const MyKinetics & kinetics, const ParamNo param_no);

	// @return parametrizations string in the form "(val_1,...,val_n)".
	string createParamString(const MyKinetics & kinetics, const ParamNo param_no);

	// @return representation of the parametrization used by the database
	string makeConcise(const MyKinetics::Param & param, const string target_name);

	// @Obtain all the parametrizations ID for the given levels string
	set<ParamNo> findMatching(const MyKinetics & kinetics, const Levels & param_vals);
}