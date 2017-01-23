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

#include "../data/model_translators.hpp"
#include "../data/kinetics_translators.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Builds the kinetic parameters
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ParameterBuilder 
{
	/**
	* @brief getSingleParam creates a parameter for a single context.
	* @return
	*/
	static Kinetics::Param addSingleParam(const Model & model, const map<CompID, Levels> & all_thrs, const Levels & thrs_comb, const CompID t_ID);

	// @brief createParameters Creates a description of kinetic parameters.
	static Kinetics::Params createParameters(const Model & model, const CompID t_ID);

public:
	// @brief fillParameters   fill idividual parameter values based on user specifications.
	static vector<Kinetics::Component> build(const Model & model);
};
