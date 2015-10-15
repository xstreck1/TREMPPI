#pragma once

#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/report/statistics.hpp>

namespace ImpactHelpers
{
	// For each regulator obtain columns needed for correlation analysis
	vector<vector<size_t> > getColumnsOfRegulator(const size_t reg_i, const Levels & thresholds, const map<size_t, string> & contexts);
}