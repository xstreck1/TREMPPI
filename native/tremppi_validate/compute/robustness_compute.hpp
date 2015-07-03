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

