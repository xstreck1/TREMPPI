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
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/report/statistics.hpp>

namespace StatisticalAnalysis
{
	double mean_val(const int step_count, sqlite3pp::query & selection);
	double std_dev_val(const int step_count, const double mean, sqlite3pp::query & selection);

	double covariance(const int step_count, const double m_mean, const double o_mean, sqlite3pp::query & m_qry, sqlite3pp::query & o_qry);
	double correlaction(const double covariance, const double m_std, const double o_std);
}
