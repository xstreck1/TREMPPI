#pragma once

#include <tremppi_common/network/definitions.hpp>

namespace PInterpreter {
	// Obtain maximal activity levels in each of the functions
	Levels getMaxes(const Configurations & params) {
		Levels maxes;

		if (params.empty())
			throw runtime_error("No parametrization in the database.");

		// Obtain maxes
		maxes = Levels(params[0].size(), 0);
		rng::for_each(params, [&maxes](const Levels & param) {
			for (int i : cscope(param)) {
				maxes[i] = max(maxes[i], param[i]);
			}
		});

		return maxes;
	}
}