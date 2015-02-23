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

// The information for the regulatory network
struct FunData {
	// Info about this component
	const RegInfo & info;
	// Statistical data
	double mean;
	double std_dev;
};
using FunsData = vector<FunData>;

/*
struct Par {
	// Common to all the sets of parametrizations
	vector<string> columns_name;
	vector<size_t> params_ids;
	// Read from the database
	vector<ParamNo> row_ids;
	Configurations params;
	// Lattice data
	NeighLattice neigh_lattice;
	vector<ParamNo> neigh_count;
	vector<vector<ParamNo> > components;
	vector<ParamNo> component_membership;
};*/