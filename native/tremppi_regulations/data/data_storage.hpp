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

#include <tremppi_common/network/definitions.hpp>

// The information for the regulatory network
struct RegData 
{
	// Info about this component
	const RegInfo & _info;
	// regulationsion graph data - for each regulating component hold the values in the order of the thresholds
	map<CompID, vector<double> > reg_corr;
	map<CompID, vector<double> > reg_freq;
	map<CompID, vector<char> > reg_sign;
	map<CompID, double> expected_freq; // Expected frequency is shared for all the edges
	RegData(const RegInfo & info) : _info(info) 
	{
		for (const auto & regulator : _info.regulators) {
			reg_corr.insert({ regulator.first, vector<double>(regulator.second.size(), 0.0) });
			reg_freq.insert({ regulator.first, vector<double>(regulator.second.size(), 0.0) });
			reg_sign.insert({ regulator.first, vector<char>(regulator.second.size(), 0) });
			expected_freq.insert({ regulator.first, 0.0 });
		}
	}
};
using RegsData = vector<RegData>;

