#pragma once

#include <tremppi_common/network/definitions.hpp>

#include "../analysis/statistics.hpp"
#include "../data/data_storage.hpp"

namespace StatisticalAnalysis {
	// Compute the correlation between the two regulatory functions in the parametrization space
	/*vector<double> functionCorrelation(const size_t param_count, const Regs & regs, const Reg & current) {
		vector<double> result;
		
		// Compute correlation with all the predecessors
		for (const size_t o_ID : crange(current.ID)) {
			double divident = Statistics::expected_prod_val(current.deviations, regs[o_ID].deviations);
			double divisor = (current.std_dev * regs[o_ID].std_dev);
			double correlation = (divisor == 0.) ? 0. : (divident / divisor);

			result.emplace_back(correlation);
		}

		return result;
	}*/

	namespace {
		double mean_val(const int step_count, sqlite3pp::query & selection) {
			double result = 0;
			for (const auto & row : selection) {
				Levels params;
				for (const int i : crange(selection.column_count()))
					params.emplace_back(row.get<ActLevel>(i));
				result += Statistics::expected_val(params);
			}
			return result / step_count;
		}

		double std_dev_val(const int step_count, const double mean, sqlite3pp::query & selection) {
			double result = 0;
			for (const auto & row : selection) {
				Levels params;
				for (const int i : crange(selection.column_count()))
					params.emplace_back(row.get<ActLevel>(i));
				result += pow(Statistics::expected_val(params) - mean, 2);
			}
			return pow(result / step_count, 0.5);
		}
	}

	// Compute the statistical values
	/*void makeRegsStats(const size_t param_count, Regs & regs) {
		for (Reg & reg : regs) {
			// Compute mean
			vector<double> means(param_count, 0);
			transform(WHOLE(reg.params), means.begin(), Statistics::expected_val<ActLevel>);
			reg.mean = Statistics::expected_val(means);

			// Compute std deviation
			reg.deviations = Statistics::deviations(means, reg.mean);
			reg.std_dev = pow(Statistics::variance(reg.deviations), 0.5);

			// Compute correlations with other functions
			reg.correlations = functionCorrelation(param_count, regs, reg);
		}
	}*/

	FunsData diff(const FunsData & sel, const FunsData & cmp) {
		FunsData dif;
		for (const CompID ID : cscope(sel)) {
			dif.emplace_back(FunData({ sel[ID].info }));
			// Compute mean
			dif[ID].mean = sel[ID].mean - cmp[ID].mean;
		}
		return dif;
	}

	FunData build(const RegInfo & info, const int step_count, sqlite3pp::query & qry) {
		FunData result = { info };
		result.mean = mean_val(step_count, qry);
		qry.reset();
		result.std_dev = std_dev_val(step_count, result.mean, qry);

		return result;
	}
}
