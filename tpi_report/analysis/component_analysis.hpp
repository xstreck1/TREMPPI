#pragma once

#include <tpi_common/network/definitions.hpp>

#include "../data/data_storage.hpp"

// TODO: Does not work for MV
namespace ComponentAnalysis {
	//
	vector<tuple<ParamNo, ParamNo, double> > stabilityVals(const vector<ParamNo> & neigh_count, const vector<vector<ParamNo> > & components) {
		vector<tuple<ParamNo, ParamNo, double> > result(components.size());

		for (const size_t comp_no : cscope(components)) {
			double stability_sum = 0.;
			ParamNo min_stability = numeric_limits<ParamNo>::max();
			ParamNo max_stability = numeric_limits<ParamNo>::min();
			for (const ParamNo param_no : components[comp_no]) {
				ParamNo stability = neigh_count[param_no];
				min_stability = min(min_stability, stability);
				max_stability = max(max_stability, stability);
				stability_sum += stability;
			}
			double stability_mean = stability_sum / components[comp_no].size();
			result[comp_no] = make_tuple(min_stability, max_stability, stability_mean);
		}

		return result;
	}

	// 
	vector<vector<ParamNo>> stableSet(const vector<ParamNo> & neigh_count, const vector<vector<ParamNo> > & components, const vector<tuple<ParamNo, ParamNo, double> > & stability_vals) {
		vector<vector<ParamNo>> result(components.size());

		for (const size_t comp_no : cscope(components)) {
			ParamNo max_stability = get<1>(stability_vals[comp_no]);
			vector<ParamNo> has_max_stability;
			copy_if(WHOLE(components[comp_no]), back_inserter(has_max_stability), [&neigh_count, max_stability](const ParamNo param_no){
				return neigh_count[param_no] == max_stability;
			});
			result[comp_no] = move(has_max_stability);
		}

		return result;
	}
}