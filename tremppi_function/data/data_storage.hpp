#pragma once

#include <tremppi_common/header.h>

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