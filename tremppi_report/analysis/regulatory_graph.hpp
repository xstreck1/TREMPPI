#pragma once

#include <tremppi_common/network/definitions.hpp>

#include "../analysis/statistics.hpp"
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
	vector<Levels> getContextValues(const Levels & param, map<ActLevel, size_t> & my_trsh_to_index, const size_t reg_no, const vector<Levels> & reg_values) {
		vector<Levels> context_values(my_trsh_to_index.size());
		for (const size_t param_i : cscope(param)) {
			ActLevel threshold = reg_values[reg_no][param_i];
			context_values[my_trsh_to_index[threshold]].emplace_back(param[param_i]);
		}
		return context_values;
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
			size_t reg_no = DataInfo::getRegulatorI(regulator.first, reg_info);
			reg_values.emplace_back(DataConv::getThrFromContexts(columns_list, reg_no));
			threshold_to_index.insert({ regulator.first, DataConv::getThresholdToIndex(regulator.second) });
		}
		auto columns_dist = DataInfo::getColumnsOfThresholds(reg_info);
		auto edge_dist = DataConv::getColumnsOfEdges(columns_dist);

		// Compute relation to each regulation of this component
		logging.newPhase("Graph build", step_count);
		for (const auto & row : qry) {
			// Obtain the data
			vector<ActLevel> param(qry.column_count());
			for (const int i : crange(qry.column_count()))
				param[i] = row.get<ActLevel>(i);

			// Compute the correlation and frequency
			for (const auto & regulator : reg_info.regulators) {
				// Obtain the id of the regulator from the order in the regulator list and prepare the results
				size_t reg_no = DataInfo::getRegulatorI(regulator.first, reg_info);
				result.reg_corr.insert({ regulator.first, vector<double>(regulator.second.size(), 0.0) });
				auto & reg_corr = result.reg_corr[regulator.first];
				result.reg_freq.insert({ regulator.first, vector<double>(regulator.second.size(), 0.0) });
				auto & reg_freq = result.reg_freq[regulator.first];

				auto context_values = getContextValues(param, threshold_to_index[regulator.first], reg_no, reg_values);
				
				for (size_t edge_no = 0; edge_no < regulator.second.size(); edge_no++) {
					double correlation = Statistics::correlation(reg_values[reg_no], param, edge_dist[regulator.first][edge_no], reg_info.columns.begin()->first);
					reg_corr[edge_no] += correlation;
					reg_freq[edge_no] += isEdgeFunctional(context_values, edge_no);
				}
				logging.step();
			}
		}

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

	map<CompID, vector<double> > substractEdgeData(const map<CompID, vector<double> > & A, const map<CompID, vector<double> > & B) {
		map<CompID, vector<double> > result;

		for (auto & reg_A : A) {
			auto & reg_B = B.at(reg_A.first);
			vector<double> diff(reg_A.second.size());
			for (const size_t val_i : cscope(reg_A.second))
				diff[val_i] = reg_A.second[val_i] - reg_B[val_i];
			result.insert({ reg_A.first, diff });
		}

		return result;
	}

	RegsData diff(const RegsData & sel, const RegsData & cmp) {
		RegsData dif;
		for (const CompID ID : cscope(sel)) {
			dif.emplace_back(RegData({ sel[ID].info }));
			dif[ID].reg_corr = substractEdgeData(sel[ID].reg_corr, cmp[ID].reg_corr);
			dif[ID].reg_freq = substractEdgeData(sel[ID].reg_freq, cmp[ID].reg_freq);
		}
		return dif;
	}
}