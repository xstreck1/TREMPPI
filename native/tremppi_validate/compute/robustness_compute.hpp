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

#include "witness_searcher.hpp"
#include "checker_setting.hpp"
#include "model_checker.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Class responsible for computation of robustness values for each acceptable parametrization.
///
/// @attention The robustness actually counts one state after the last measurement in the time series.
/// This is however in order since the penultimate state can undergo all the transitions and therefore the robustness just gets split in between the final states.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RobustnessCompute 
{
	const ProductStructure & product; ///< Product reference for state properties.
	vector<double> prob; ///< Probability storage for the whole computation

public:
	/**
	 * Constructor ensures that data objects used within the whole computation process have appropriate size.
	 */
	RobustnessCompute(const ProductStructure & _product);

	/**
	 * Function that computes robustness values for each parametrization.
	 */
	double compute(const CheckerSetting & _settings, const Levels & _parametrization, const VisitStorage & results, const multimap<StateID, StateID> & transitions);
};

