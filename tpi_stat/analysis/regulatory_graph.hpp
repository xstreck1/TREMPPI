#pragma once
#include <tpi_common/database_reader.hpp>
#include <tpi_common/logging.hpp>
#include <tpi_common/data_conv.hpp>
#include "../analysis/statistics.hpp"
#include "../data/data_storage.hpp"

namespace RegulatoryGraph {
	// computes the correlation of the regulatiors to the regulated component for each component
	RegData build(const RegInfo & info, const int step_count, query & qry) {
		RegData result = { info };
		result.reg_corr = result.reg_freq = vector<double>(info.regulators.size());

		// Get the header data
		vector<Levels> reg_values;
		vector<string> columns_list = DataConv::columns2list(info.columns);
		for (const auto & regulator : info.regulators) {
			size_t reg_no = distance(begin(info.regulators), info.regulators.find(regulator.first));
			reg_values.emplace_back(DatabaseReader::getThrFromContexts(columns_list, reg_no));
		}

		// Compute relation to each regulation of this component
		Logging::newPhase(step_count, "Graph build");
		for (const auto & row : qry) {
			// Obtain the data
			vector<ActLevel> param(qry.column_count());
			for (const int i : crange(qry.column_count()))
				param[i] = row.get<ActLevel>(i);

			// Compute the correlation
			size_t reg_i = 0;
			for (const size_t reg_i : cscope(info.regulators)) {
				double correlation = Statistics::correlation(reg_values[reg_i], param);
				result.reg_corr[reg_i] += correlation;
				result.reg_freq[reg_i] += (correlation != 0.);
			}
			Logging::step();
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