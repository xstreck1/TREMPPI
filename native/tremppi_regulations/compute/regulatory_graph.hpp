#pragma once

#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/report/statistics.hpp>

#include "../data/data_storage.hpp"

namespace RegulatoryGraph 
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

	// computes the correlation of the regulators to the regulated component for each component
	void compute(const RegInfos & reg_infos, const CompID ID, const int step_count, sqlite3pp::query & qry, map<CompID, vector<double> > & corr) 
	{
		map<Regulation, size_t> column_of_regulation;
		for (const auto & regulator : reg_infos[ID].regulators) 
		{
			for (ActLevel threshold : regulator.second) {
				string column_name = "I_" + reg_infos[regulator.first].name + "_" + to_string(threshold) + "_" + reg_infos[ID].name;
				Regulation regulation = Regulation{ reg_infos[regulator.first].ID, threshold, reg_infos[ID].ID };
				for (const size_t column_i : crange(qry.column_count())) 
				{
					string database_column = qry.column_name(column_i);
					if (database_column == column_name) 
					{
						column_of_regulation[regulation] = column_i;
					}
				}
				if (column_of_regulation.count(regulation) == 0) 
				{
					throw runtime_error("did not find the regulation " + column_name + " in the database");
				}
			}
		}

		// Compute relation to each regulation of this component, step by database rows
		for (const auto & row : qry) 
		{
			for (const auto & regulator : reg_infos[ID].regulators) {
				for (size_t threshold_i : cscope(regulator.second)) 
				{
					const ActLevel threshold = regulator.second[threshold_i];
					Regulation regulation = Regulation{ reg_infos[regulator.first].ID, threshold, reg_infos[ID].ID };
					size_t column_i = column_of_regulation[regulation];
					double impact = row.get<double>(column_i);
					corr[regulator.first][threshold_i] += impact;
				}
			}
		}
		
		RegData::Normalize(step_count, corr);
	}
}