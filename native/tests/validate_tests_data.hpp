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

#include <tremppi_validate/data/construction_manager.hpp>

#include "common_tests_data.hpp"


class ValidateTest : public CommonTest 
{
protected:
	PropertyInfo a_spike_on_A;
	PropertyInfo a_is_steady;
	PropertyInfo a_cycle_on_A;


	void SetUp() override 
	{
		CommonTest::SetUp();

		a_spike_on_A.name = "spike_on_A";
		a_spike_on_A.ending = "open";
		a_spike_on_A.measurements.emplace_back(PropertyInfo::Measurement{0, {{"A", {0, 0}}}, {}});
		a_spike_on_A.measurements.emplace_back(PropertyInfo::Measurement{1, {{"A", {1, 1}}}, {}});
		a_spike_on_A.measurements.emplace_back(PropertyInfo::Measurement{2, {{"A", {0, 0}}}, {}});
		a_spike_on_A.bound = INF;

		a_is_steady.name = "is_steady";
		a_is_steady.ending = "stable";
		a_is_steady.measurements.emplace_back(PropertyInfo::Measurement{ 0,{},{} });
		a_is_steady.bound = INF;

		a_cycle_on_A.name = "cycle_on_A";
		a_cycle_on_A.ending = "cyclic";
		a_cycle_on_A.measurements.emplace_back(PropertyInfo::Measurement{ 1,{ { "A",{ 1,1 } } },{} });
		a_cycle_on_A.measurements.emplace_back(PropertyInfo::Measurement{ 2,{ { "A",{ 0,0 } } },{} });
		a_cycle_on_A.bound = INF;
	}
};
