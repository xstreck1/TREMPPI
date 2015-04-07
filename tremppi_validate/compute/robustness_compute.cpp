#include "robustness_compute.hpp"

RobustnessCompute::RobustnessCompute(const ProductStructure & _product) : product(_product) {
	prob = vector<double>(product.getStateCount(), 0.0);
}

double RobustnessCompute::compute(const CheckerSetting & _settings, const Levels & _parametrization, const VisitStorage & results, const multimap<StateID, StateID>& transitions) {
	const vector<StateID> & initials = _settings.getInitials(product);

	prob.assign(prob.size(), 0.0);
	for (const StateID init : _settings.getInitials(product))
		prob[init] = 1.0 / initials.size();

	vector<StateID> updates(WHOLE(initials));
	vector<StateID> next_updates;

	// Cycle through the levels of the BFS procedure
	for (size_t round_num = 0; round_num < results.getCost(); round_num++) {
		for (const StateID ID : updates) {
			auto transports = ModelChecker::broadcastParameters(_parametrization, product, ID).first;
			double exit_prob = prob[ID] / transports.size();

			for (auto it = transitions.equal_range(ID).first; it != transitions.equal_range(ID).second; it++) {
				if (prob[it->second] == 0.0)
					next_updates.push_back(it->second);
				prob[it->second] += exit_prob;
			}
		}

		updates = move(next_updates);
	}

	double robustness = 0.0;
	for (const StateID ID : _settings.getFinals(product))
		robustness += prob[ID];
	return robustness;
}
