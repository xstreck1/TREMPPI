#pragma once

#include <tremppi_common/network/data_info.hpp>

// The information for the regulatory network
struct FunData 
{
	// Info about this component
	const RegInfo & info;
	// Statistical data
	double mean;
	double std_dev;
	vector<double> corrs;
};
using FunsData = vector<FunData>;
