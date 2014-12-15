#pragma once

#include "witness_searcher.hpp"
#include "checker_setting.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Class responsible for computation of robustness values for each acceptable parametrization.
///
/// @attention The robustness actually counts one state after the last measurement in the time series.
/// This is however in order since the penultimate state can undergo all the transitions and therefore the robustness just gets split in between the final states.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RobustnessCompute {
	const ProductStructure & product; ///< Product reference for state properties.

public:
	/**
	 * Constructor ensures that data objects used within the whole computation process have appropriate size.
	 */
	RobustnessCompute(const ProductStructure & _product) : product(_product) {   }

	/**
	 * Function that computes robustness values for each parametrization.
	 */
	double compute(const CheckerSetting & _settings, const Levels & _parametrization, const VisitStorage & results, const multimap<StateID, StateID> & transitions) {
		const vector<StateID> & initials = _settings.getInitials(product);
		
		vector<double> prob = vector<double>(product.getStateCount(), 0.0);
		for (const StateID init : _settings.getInitials(product))
			prob[init] = 1.0 / initials.size();

		vector<StateID> updates = initials;
		vector<StateID> next_updates;

		// Cycle through the levels of the DFS procedure
		for (size_t round_num = 0; round_num < results.getCost(); round_num++) {
			next_updates.clear();

			for (const StateID ID : updates) {
				auto transports = SuccFunc::broadcastParameters(_parametrization, product, ID);
				double exit_prob = prob[ID] / transports.size();

				for (auto it = transitions.equal_range(ID).first; it != transitions.equal_range(ID).second; it++) {
					next_updates.push_back(it->second);
					prob[it->second] += exit_prob;
				}
			}

			auto unique_it = unique(WHOLE(next_updates));
			next_updates.resize(distance(begin(next_updates), unique_it));
			updates = move(next_updates);
		}

		double robustness = 0.0;
		for (const StateID ID : _settings.getFinals(product))
			robustness += prob[ID];
		return robustness;
	}
};

