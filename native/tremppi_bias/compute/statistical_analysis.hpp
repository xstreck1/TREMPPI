#pragma once

#include <tremppi_common/network/definitions.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/report/statistics.hpp>

#include "../data/data_storage.hpp"

namespace StatisticalAnalysis {

	namespace {
		double fun_bias(const sqlite3pp::query::rows & row) {
			Levels params;
			for (const int i : crange(row.column_count()))
				params.emplace_back(row.get<ActLevel>(i));
			return Statistics::expected_val(params);
		}

		double mean_val(const int step_count, sqlite3pp::query & selection) {
			double result = 0;
			for (const auto & row : selection)
				result += fun_bias(row);
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

	double covariance(const int step_count, const double m_mean, const double o_mean, sqlite3pp::query & m_qry, sqlite3pp::query & o_qry) {
		double result = 0;

		auto m_row = m_qry.begin();
		auto o_row = o_qry.begin();
		while (m_row != m_qry.end()) {
			double m_bias = fun_bias(*m_row);
			double o_bias = fun_bias(*o_row);

			result += (m_bias - m_mean)*(o_bias - o_mean);

			m_row++; o_row++;
		}

		return result / step_count;
	}

	double correlaction(const double covariance, const double m_std, const double o_std) {
		if (m_std == 0 || o_std == 0) {
			return 0;
		}
		else {
			return covariance / (m_std * o_std);
		}
	}

	FunsData diff(const FunsData & sel, const FunsData & cmp) {
		FunsData dif;
		for (const CompID ID : cscope(sel)) {
			dif.emplace_back(FunData({ sel[ID].info }));
			// Compute mean
			dif[ID].mean = sel[ID].mean - cmp[ID].mean;
			dif[ID].std_dev = sel[ID].std_dev - cmp[ID].std_dev;
			for (const int i : cscope(sel[ID].corrs))
				dif[ID].corrs.push_back(sel[ID].corrs[i] - cmp[ID].corrs[i]);
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
