#pragma once

#include <tremppi_common/network/definitions.hpp>

#include "../analysis/statistics.hpp"
#include "../data/data_storage.hpp"

namespace RegulatoryGraph {
	// Returns true if form the regulator targets (for each threshold, all the target values, ordered) the edge appears as functional
	bool isEdgeFunctional(const map<ActLevel, Levels> & context_values, const Levels & thresholds) { 
		for (const size_t config_i : cscope(context_values.at(0))) {
			for (const size_t thrs_i : crange(0u, thresholds.size())) {
				ActLevel bottom = thrs_i == 0 ? 0 : thresholds[thrs_i - 1];
				if (context_values.at(bottom)[config_i] != context_values.at(thresholds[thrs_i])[config_i])
					return true;
			}
		}

		return false;
	}

	// computes the correlation of the regulatiors to the regulated component for each component
	RegData build(const RegInfo & info, const int step_count, sqlite3pp::query & qry) {
		RegData result = { info };
		result.reg_corr = result.reg_freq = vector<double>(info.regulators.size());

		// Get the header data
		vector<Levels> reg_values;
		vector<string> columns_list = DataConv::columns2list(info.columns);
		for (const auto & regulator : info.regulators) {
			// Obtain the id of the regulator from the order in the regulator list
			size_t reg_no = distance(begin(info.regulators), info.regulators.find(regulator.first));
			reg_values.emplace_back(DataConv::getThrFromContexts(columns_list, reg_no));
		}

		// Compute relation to each regulation of this component
		logging.newPhase("Graph build", step_count);
		for (const auto & row : qry) {
			// Obtain the data
			vector<ActLevel> param(qry.column_count());
			for (const int i : crange(qry.column_count()))
				param[i] = row.get<ActLevel>(i);

			// Compute the correlation
			for (const auto & regulator : info.regulators) {
				// Obtain the id of the regulator from the order in the regulator list
				size_t reg_no = distance(begin(info.regulators), info.regulators.find(regulator.first));

				double correlation = Statistics::correlation(reg_values[reg_no], param);
				result.reg_corr[reg_no] += correlation;
				map<ActLevel, Levels> context_values;
				// Compute the distribution of target values
				for (const size_t param_i : cscope(param)) {
					ActLevel threshold = reg_values[reg_no][param_i];
					context_values[threshold].emplace_back(param[param_i]);
				}
				result.reg_freq[reg_no] += isEdgeFunctional(context_values, regulator.second);

				logging.step();
			}
		}

		// Normalize by the number of parametrizations
		for (const size_t reg_i : cscope(info.regulators)) {
			result.reg_corr[reg_i] /= step_count;
			result.reg_freq[reg_i] /= step_count;
		}
		return result;
	}

	RegsData diff(const RegsData & sel, const RegsData & cmp) {
		RegsData dif;
		for (const CompID ID : cscope(sel)) {
			dif.emplace_back(RegData({ sel[ID].info }));

			transform(WHOLE(sel[ID].reg_corr), begin(cmp[ID].reg_corr), back_inserter(dif[ID].reg_corr), minus<double>());
			transform(WHOLE(sel[ID].reg_freq), begin(cmp[ID].reg_freq), back_inserter(dif[ID].reg_freq), [](const double a, const double b){
				return abs(a) - abs(b);
			});
		}
		return dif;
	}
}