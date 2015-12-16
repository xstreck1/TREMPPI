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

#include "unparametrized_structure.hpp"

StateID UnparametrizedStructure::computeID(const Levels & levels) const 
{
	StateID result = 0;
	size_t factor = 1;

	for (size_t lvl_no = 0; lvl_no < levels.size(); lvl_no++) 
	{
		result += (levels[lvl_no] - get<0>(_bounds)[lvl_no]) * factor;
		factor *= (get<2>(_bounds)[lvl_no]);
	}

	return result;
}
