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

#include "../data/data.hpp"

namespace MVQMC
{
	/* Return a vector of combined values, if possible, otherwise return an empty vector. */
	PMin combine(PMin & a, PMin & b);

	/* Return a vector of combinations of the next order. Those who were not possible to merge are left in the reference to input. */
	vector<PMin> next_merge(vector<PMin> & original);

	vector<PMin> valuesToCompat(vector<vector<int> > original);

	/* Create representation of the vector with don't cares. */
	vector<PMin> compactize(vector<PMin> current);
}