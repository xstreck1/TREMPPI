#pragma once

#include <tremppi_common/network/definitions.hpp>
#include <tremppi_common/report/statistics.hpp>

#include "../data/data_storage.hpp"

namespace RegulatoryGraph {
	// Retrun true if the edge is functional
	bool isEdgeFunctional(const vector<Levels> & context_values, const size_t & edge_no) {
		// Compare the values in this and following thesholds, should go from 0 value
		for (const size_t config_i : cscope(context_values[0]))
			if (context_values[edge_no][config_i] != context_values[edge_no + 1][config_i])
				return true;

		return false;
	}

	// Compute the distribution of target values
	vector<Levels> getContextValues(const Levels & param, const map<ActLevel, size_t> & my_trsh_to_index, const size_t reg_no, const vector<Levels> & reg_values) {
		vector<Levels> context_values(my_trsh_to_index.size());
		for (const size_t param_i : cscope(param)) {
			ActLevel threshold = reg_values[reg_no][param_i];
			context_values[my_trsh_to_index.at(threshold)].emplace_back(param[param_i]);
		}
		return context_values;
	}

	// Compute data for a single parametrization row
	void compute(const map<CompID, vector<vector<size_t>>> & edge_dist, const map<CompID, map<ActLevel, size_t> > & threshold_to_index, const vector<Levels> & reg_values,
		const RegInfo & reg_info, const sqlite3pp::query::rows & row, const size_t column_count, RegData & result) {
		// Obtain the data
		vector<ActLevel> param(column_count);
		for (const int i : crange(column_count))
			param[i] = row.get<ActLevel>(i);

		// Compute the correlation and frequency
		for (const auto & regulator : reg_info.regulators) {
			// Obtain the id of the regulator from the order in the regulator list and prepare the results
			size_t reg_no = DataInfo::RegIDToRegNo(reg_info, regulator.first);
			result.reg_corr.insert({ regulator.first, vector<double>(regulator.second.size(), 0.0) });
			auto & reg_corr = result.reg_corr[regulator.first];
			result.reg_freq.insert({ regulator.first, vector<double>(regulator.second.size(), 0.0) });
			auto & reg_freq = result.reg_freq[regulator.first];

			auto context_values = getContextValues(param, threshold_to_index.at(regulator.first), reg_no, reg_values);

			for (size_t edge_no = 0; edge_no < regulator.second.size(); edge_no++) {
				double correlation = Statistics::correlation(reg_values[reg_no], param, edge_dist.at(regulator.first)[edge_no], reg_info.columns.begin()->first);
				reg_corr[edge_no] += correlation;
				reg_freq[edge_no] += isEdgeFunctional(context_values, edge_no);
			}
		}
	}


	map<CompID, vector<vector<size_t>>> getColumnsOfThresholds(const RegInfo & reg_info)
	{
		map<CompID, vector<vector<size_t>>> result;

		for (auto & regulator : reg_info.regulators) {
			vector<vector<size_t> > columns(regulator.second.size() + 1);
			const size_t reg_i = DataInfo::RegIDToRegNo(reg_info, regulator.first);
			map<ActLevel, size_t> threshold_to_index = DataConv::getThresholdToIndex(regulator.second);

			// Distribute the column indices based on the value of the threshold in the specific column
			for (auto & column : reg_info.columns) {
				const auto & trhs = DataConv::getThrsFromContext(column.second);
				ActLevel trh = trhs[reg_i];
				size_t i = threshold_to_index[trh];
				columns[i].push_back(column.first);
			}

			result.insert({ regulator.first, move(columns) });
		}

		return result;
	}

	map<CompID, vector<vector<size_t>>> getColumnsOfEdges(const map<CompID, vector<vector<size_t>>> & columns_of_thresholds) {
		map<CompID, vector<vector<size_t> > > result;

		for (const auto & source_columns : columns_of_thresholds) {
			vector<vector<size_t>> edge_columns(source_columns.second.size() - 1);

			for (size_t trhs_column_i = 0; trhs_column_i < edge_columns.size(); trhs_column_i++) {
				edge_columns[trhs_column_i] = source_columns.second[trhs_column_i] + source_columns.second[trhs_column_i + 1];
			}

			result.insert({ source_columns.first, edge_columns });
		}

		return result;
	}

	// Divide the number of parametrizations in total by the number of parametrizations sharing the same regulatory function
	double computeExpectedFreq(const RegInfo & reg_info, const CompID reg_ID) {
		const size_t context_c = accumulate(WHOLE(reg_info.regulators), 1, [](const size_t part_val, const pair<CompID, Levels> & regulator){
			return part_val * (regulator.second.size() + 1);
		});
		const size_t other_c = context_c / (reg_info.regulators.at(reg_ID).size() + 1);
		double all_params = pow(reg_info.max_activity + 1, context_c);
		double same_params = pow(reg_info.max_activity + 1, other_c) * pow(reg_info.max_activity + 1, other_c * (reg_info.regulators.at(reg_ID).size() - 1));

		return 1 - (same_params / all_params);
	}

	// computes the correlation of the regulators to the regulated component for each component
	RegData build(const RegInfo & reg_info, const int step_count, sqlite3pp::query & qry) {
		RegData result = { reg_info };

		// Get the header data
		vector<string> columns_list = DataConv::columns2list(reg_info.columns);
		vector<Levels> reg_values; // For each regulator holds the values of the threshold in each column
		map<CompID, map<ActLevel, size_t> > threshold_to_index; // Mapper from thresholds to their order, for each regulator
		for (const auto & regulator : reg_info.regulators) {
			// Obtain the id of the regulator from the order in the regulator list
			size_t reg_no = DataInfo::RegIDToRegNo(reg_info, regulator.first);
			reg_values.emplace_back(DataConv::getThrFromContexts(columns_list, reg_no));
			threshold_to_index.insert({ regulator.first, DataConv::getThresholdToIndex(regulator.second) });
			// Compute the expected frequency for this regulator
			result.expected_freq[regulator.first] = computeExpectedFreq(reg_info, regulator.first);
		}
		auto columns_dist = getColumnsOfThresholds(reg_info);
		auto edge_dist = getColumnsOfEdges(columns_dist);

		// Compute relation to each regulation of this component, step by database rows
		for (const auto & row : qry) 
			compute(edge_dist, threshold_to_index, reg_values, reg_info, row, qry.column_count(), result);

		// Normalize by the number of parametrizations
		auto normalize = [step_count](map<CompID, vector<double>> & values) {
			for (auto & reg_val : values) {
				for (auto & edge_val : reg_val.second) {
					edge_val /= step_count;
				}
			}
		};
		normalize(result.reg_corr);
		normalize(result.reg_freq);

		return result;
	}
}