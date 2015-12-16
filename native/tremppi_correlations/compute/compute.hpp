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

#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/report/report.hpp>
#include <tremppi_common/general/logging.hpp>

#include "statistical_analysis.hpp"
#include "../data/data_storage.hpp"

namespace Compute
{
	FunData build(const RegInfo & info, const int step_count, sqlite3pp::query & qry);

	void deviation(const RegInfos & reg_infos, const size_t step_count, vector<sqlite3pp::query> & queries, Logging & logging, FunsData & funs_data);

	void correlation(const RegInfos & reg_infos, const size_t step_count, vector<sqlite3pp::query> & queries, Logging & logging, FunsData & funs_data);
}