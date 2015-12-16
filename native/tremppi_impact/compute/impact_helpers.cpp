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

#include "impact_helpers.hpp"

namespace ImpactHelpers
{
	vector<vector<size_t> > getColumnsOfRegulator(const size_t reg_i, const Levels & thresholds, const map<size_t, string> & contexts)
	{
		vector<vector<size_t> > result(thresholds.size());

		// Distribute the column indices based on the value of the threshold in the specific column
		vector<vector<size_t> > threshold_columns(thresholds.size() + 1);
		map<ActLevel, size_t> threshold_to_index = DataConv::getThresholdToIndex(thresholds);
		for (auto & context : contexts)
		{
			ActLevel trh = context.second[reg_i];
			size_t i = threshold_to_index[trh];
			threshold_columns[i].push_back(context.first);
		}

		// Join below and above threshold columns
		for (size_t trhs_column_i : cscope(result))
		{
			result[trhs_column_i] = threshold_columns[trhs_column_i] + threshold_columns[trhs_column_i + 1];
		}
		return result;
	}
}