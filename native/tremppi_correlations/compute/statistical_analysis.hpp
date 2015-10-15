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
