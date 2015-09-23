#pragma once

#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/report/statistics.hpp>

namespace ImpactHelpers
{
	// For each regulator obtain columns needed for correlation analysis
	vector<vector<size_t> > getColumnsOfRegulator(const size_t reg_i, const Levels & thresholds, const map<size_t, string> & contexts)
	{
		vector<vector<size_t> > result(thresholds.size());

		// Distribute the column indices based on the value of the threshold in the specific column
		vector<vector<size_t> > threshold_columns(thresholds.size() + 1);
		map<ActLevel, size_t> threshold_to_index = DataConv::getThresholdToIndex(thresholds);
		for (auto & context : contexts)
		{
			ActLevel trh = context.second[reg_i];
			size_t i = threshold_to_index[trh];
			threshold_columns[i].push_back(context.first);
		}

		// Join below and above threshold columns
		for (size_t trhs_column_i : cscope(result))
		{
			result[trhs_column_i] = threshold_columns[trhs_column_i] + threshold_columns[trhs_column_i + 1];
		}
		return result;
	}
}