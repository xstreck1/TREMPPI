/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#pragma once

#include "kinetics.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Functions that take take in the Kinetics data structure and compute new data from them.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace KineticsTranslators 
{
	// @return the number of parametrizations
	ParamNo getSpaceSize(const Kinetics & kinetics);

	// @return parametrization vector from its number
	Levels createParamVector(const Kinetics & kinetics, const ParamNo param_no);

	// @return 
	Levels getSpecieVals(const Kinetics & kinetics, ParamNo param_no);

	// @return parametrizations string in the form "(val_1,...,val_n)".
	string createParamString(const Kinetics & kinetics, const ParamNo param_no);

	// @return representation of the parametrization used by the database
	string makeConcise(const Kinetics::Param & param, const string target_name);
}