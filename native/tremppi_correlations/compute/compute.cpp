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

#include "compute.hpp"

FunData Compute::build(const RegInfo & info, const int step_count, sqlite3pp::query & qry)
{
	FunData result = { info };
	result.mean = StatisticalAnalysis::mean_val(step_count, qry);
	qry.reset();
	result.std_dev = StatisticalAnalysis::std_dev_val(step_count, result.mean, qry);

	return result;
}

void Compute::deviation(const RegInfos & reg_infos, const size_t step_count, vector<sqlite3pp::query> & queries, Logging & logging, FunsData & funs_data)
{
	logging.newPhase("Deviation of function", reg_infos.size());
	for (const RegInfo & reg_info : reg_infos)
	{
		funs_data.emplace_back(build(reg_info, step_count, queries[reg_info.ID]));
		logging.step();
	}
}

void Compute::correlation(const RegInfos & reg_infos, const size_t step_count, vector<sqlite3pp::query> & queries, Logging & logging, FunsData & funs_data)
{
	logging.newPhase("Correlating function", reg_infos.size());
	for (FunData & m_fun : funs_data)
	{
		for (const FunData & o_fun : funs_data)
		{
			if (m_fun.info.ID <= o_fun.info.ID)
			{
				continue;
			}
			queries[m_fun.info.ID].reset(); queries[o_fun.info.ID].reset();
			double covariance = StatisticalAnalysis::covariance(step_count, m_fun.mean, o_fun.mean, queries[m_fun.info.ID], queries[o_fun.info.ID]);
			m_fun.corrs.push_back(StatisticalAnalysis::correlaction(covariance, m_fun.std_dev, o_fun.std_dev));
		}
		logging.step();
	}
}