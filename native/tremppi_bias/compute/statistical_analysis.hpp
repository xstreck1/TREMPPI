#pragma once

#include <tremppi_common/network/definitions.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/report/statistics.hpp>

#include "../data/data_storage.hpp"

namespace StatisticalAnalysis 
{
	namespace {
		double mean_val(const int step_count, sqlite3pp::query & selection) 
		{
			double result = 0;
			for (const auto & row : selection)
				result += row.get<double>(0);
			return result / step_count;
		}

		double std_dev_val(const int step_count, const double mean, sqlite3pp::query & selection) 
		{
			double result = 0;
			for (const auto & row : selection) 
			{
				result += pow(row.get<double>(0) - mean, 2);
			}
			return pow(result / step_count, 0.5);
		}
	}

	double covariance(const int step_count, const double m_mean, const double o_mean, sqlite3pp::query & m_qry, sqlite3pp::query & o_qry) 
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

	double correlaction(const double covariance, const double m_std, const double o_std) 
	{
		if (m_std == 0 || o_std == 0) {
			return 0;
		}
		else 
		{
			return covariance / (m_std * o_std);
		}
	}
}
