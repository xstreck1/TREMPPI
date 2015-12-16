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

#include "statistical_analysis.hpp"

double StatisticalAnalysis::mean_val(const int step_count, sqlite3pp::query & selection)
{
	double result = 0;
	for (const auto & row : selection)
		result += row.get<double>(0);
	return result / step_count;
}

double StatisticalAnalysis::std_dev_val(const int step_count, const double mean, sqlite3pp::query & selection)
{
	double result = 0;
	for (const auto & row : selection)
	{
		result += pow(row.get<double>(0) - mean, 2);
	}
	return pow(result / step_count, 0.5);
}
double StatisticalAnalysis::covariance(const int step_count, const double m_mean, const double o_mean, sqlite3pp::query & m_qry, sqlite3pp::query & o_qry)
{
	double result = 0;

	auto m_row = m_qry.begin();
	auto o_row = o_qry.begin();
	while (m_row != m_qry.end())
	{
		double m_bias = m_row->get<double>(0);
		double o_bias = o_row->get<double>(0);

		result += (m_bias - m_mean)*(o_bias - o_mean);

		m_row++; o_row++;
	}

	return result / step_count;
}
double StatisticalAnalysis::correlaction(const double covariance, const double m_std, const double o_std)
{
	if (m_std == 0 || o_std == 0) {
		return 0;
	}
	else
	{
		return covariance / (m_std * o_std);
	}
}