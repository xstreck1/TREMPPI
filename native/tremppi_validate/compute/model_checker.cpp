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

#include "model_checker.hpp"

bool ModelChecker::isOpen(const Levels & parametrization, const TransConst & trans_const) 
{
	if (trans_const._req_dir)
		return parametrization[trans_const._param_no] > trans_const._req_value;
	else
		return parametrization[trans_const._param_no] < trans_const._req_value;
}

bool ModelChecker::isStable(const Levels & parametrization, const std::vector<StayConst> & stay_consts)
{
	for (const StayConst & stay_const : stay_consts) {
		if (parametrization[stay_const._param_no] != stay_const._req_value)
		{
			return false;
		}
	}
	return true;
}

VisitStorage ModelChecker::conductCheck(const ProductStructure & product, const CheckerSetting & _settings, const Levels & _parametrization, VisitStorage storage) 
{
	storage.reset();

	vector<StateID> updates = _settings.getInitials(product);

	if (!_settings.circ) 
	{
		for (const StateID ID : updates) {
			storage.update(ID);
		}
	}
	vector<StateID> next_updates; ///< Updates that are sheduled forn the next round.
	bool sat = false;

	// While there are updates, pass them to succesing vertices
	while (!updates.empty()) 
	{
		storage.incCost();

		for (const StateID ID : updates) 
		{
			// Get passed states and remove those already found
			vector<StateID> transports; bool stable;
			tie(transports, stable) = broadcastParameters(_parametrization, product, ID);
			auto trans_end = remove_if(WHOLE(transports), [&storage](const StateID t_ID) {return storage.isFound(t_ID); });

			for (auto next_it = begin(transports); next_it != trans_end; next_it++) 
			{
				storage.update(*next_it);
			}
			next_updates.insert(end(next_updates), begin(transports), trans_end);

			// Don't update when looking for a loop
			if (!(_settings.circ && storage.getCost() == 1)) 
			{
				if (_settings.isFinal(ID, product) && (stable || !_settings.stable))
					sat = true;
			}
		}
		if ((sat && _settings.bound_type == BoundType::bt_min) || (_settings.bfs_bound <= storage.getCost() && _settings.bound_type == BoundType::bt_step))
			updates.clear();
		else
			updates = move(next_updates);

	};

	if (!sat)
		storage.notFound();
	return storage;
}
