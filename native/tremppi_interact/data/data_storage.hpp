#pragma once

#include <tremppi_common/header.h>

enum EdgeSign { EdgePos, EdgeNeg, EdgeMixed };

// The information for the regulatory network
struct RegData {
	// Info about this component
	const RegInfo & info;
	// Interaction graph data - for each regulating component hold the values in the order of the thresholds
	map<CompID, vector<double> > reg_corr;
	map<CompID, vector<double> > reg_freq;
	map<CompID, vector<EdgeSign> > reg_sign;
	map<CompID, double> expected_freq; // Expected frequency is shared for all the edges
};
using RegsData = vector<RegData>;