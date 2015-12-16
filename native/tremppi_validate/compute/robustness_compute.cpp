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

#include "robustness_compute.hpp"

RobustnessCompute::RobustnessCompute(const ProductStructure & _product) : product(_product) 
{
	prob = vector<double>(product.size(), 0.0);
}

double RobustnessCompute::compute(const CheckerSetting & _settings, const Levels & _parametrization, const VisitStorage & results, const multimap<StateID, StateID>& transitions) 
{
	const vector<StateID> & initials = _settings.getInitials(product);

	prob.assign(prob.size(), 0.0);
	for (const StateID init : _settings.getInitials(product))
		prob[init] = 1.0 / initials.size();

	vector<StateID> updates(WHOLE(initials));
	vector<StateID> next_updates;

	// Cycle through the levels of the BFS procedure
	for (size_t round_num = 0; round_num < results.getCost(); round_num++) 
	{
		for (const StateID ID : updates) {
			auto transports = ModelChecker::broadcastParameters(_parametrization, product, ID).first;
			double exit_prob = prob[ID] / transports.size();


			for (auto it = transitions.equal_range(ID).first; it != transitions.equal_range(ID).second; it++) 
			{
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
