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

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/data_info.hpp>

enum BoundType { bt_inf, bt_min, bt_step }; //< What sort of bound the user placed - do the full check / check until the minimal lenght is found for a parametrization / use only bounded amount of steps
enum PathCons { pc_none, pc_up, pc_down, pc_stay }; ///< What are the requirements on the transitions between measurements? (none, monotonicity, stability)
struct PropertyInfo 
{
	NO_COPY(PropertyInfo);

	struct Measurement 
	{
		size_t ID; ///< Numerical constant used to distinguish the state. Starts from 0!
		map<string, ActRange> state_constraints; ///< Per component state constraints
		map<string, PathCons> path_constraints; ///< Per component path constraints
	};
	vector<Measurement> measurements; 

	string name; // Identifier for the property
	string ending; // What should happen after last measurement? (anything, stability is reached, there is return to another) 
	map<string, ActRange> bounds; // Per component Range of component activity (bound inclusive)
	// Execution constraints
	size_t bound; // Maximal search depth..., not used now
};

