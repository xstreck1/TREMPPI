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

#include "common_tests_data.hpp"

int basic_init_test();
int basic_enumerate_test();
int basic_express_test();
int basic_bias_test();
int basic_correlations_test();
int basic_update_test();
int basic_qualitative_test();
int basic_quantitative_test();
int basic_regulations_test();
int basic_impact_test();
int basic_group_test();
int basic_sign_test();
int basic_validate_test();
int basic_witness_test();

void createModel(const bfs::path & example_model_path);
void createSelect(const bfs::path & example_model_path);
void createProperties(const bfs::path & example_model_path);