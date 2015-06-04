#pragma once

#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/report/statistics.hpp>

#include "../data/data_storage.hpp"

namespace RegulatoryGraph {
	// For each regulator obtain columns needed for correlation analysis
	vector<vector<size_t> > getColumnsOfRegulator(const size_t reg_i, const Levels & thresholds, const map<size_t, string> & contexts) {
		vector<vector<size_t> > result(thresholds.size());

		// Distribute the column indices based on the value of the threshold in the specific column
		vector<vector<size_t> > threshold_columns(thresholds.size() + 1);
		map<ActLevel, size_t> threshold_to_index = DataConv::getThresholdToIndex(thresholds);
		for (auto & context : contexts) {
			ActLevel trh = context.second[reg_i];
			size_t i = threshold_to_index[trh];
			threshold_columns[i].push_back(context.first);
		}

		// Join below and above threshold columns
		for (size_t trhs_column_i : cscope(result)) {
			result[trhs_column_i] = threshold_columns[trhs_column_i] + threshold_columns[trhs_column_i + 1];
		}

		return result;
	}

	// computes the correlation of the regulators to the regulated component for each component
	void compute(const RegInfo & reg_info, const int step_count, sqlite3pp::query & qry, map<CompID, vector<double> > & result) {
		// Get the header data
		vector<string> columns_list = DataConv::columns2list(reg_info.columns);
		// For each regulator holds the values of the threshold in each column
		vector<Levels> reg_values;
		// For each regulator hold the columns that contain the contexts with this regulator and those exactly without is (i.e. if I have a context with this regulator and remove the regulator, what do I obtain?)
		map<CompID, vector<vector<size_t>>> edge_dist;
		for (const auto & regulator : reg_info.regulators) {
			// Obtain the id of the regulator from the order in the regulator list
			size_t reg_i = DataInfo::RegIDToRegNo(reg_info, regulator.first);
			reg_values.emplace_back(DataConv::getThrFromContexts(columns_list, reg_i));
			edge_dist.insert({ regulator.first, getColumnsOfRegulator(reg_i, regulator.second, reg_info.columns) });
		}

		// Compute relation to each regulation of this component, step by database rows
		for (const auto & row : qry) {
			Levels parametrization = sqlite3pp::func::getRow<ActLevel>(row, 0, qry.column_count());

			for (const auto & regulator : reg_info.regulators) {
				// Obtain the id of the regulator from the order in the regulator list and prepare the results
				size_t reg_no = DataInfo::RegIDToRegNo(reg_info, regulator.first);

				for (size_t edge_no = 0; edge_no < regulator.second.size(); edge_no++) {
					double correlation = Statistics::correlation(reg_values[reg_no], parametrization, edge_dist.at(regulator.first)[edge_no], reg_info.columns.begin()->first);
					result[regulator.first][edge_no] += correlation;
				}
			}
		}

		RegData::Normalize(step_count, result);
	}
}