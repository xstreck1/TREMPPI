#pragma once

#include <tremppi_common/network/definitions.hpp>

// The information for the regulatory network
struct RegData 
{
	// Info about this component
	const RegInfo & _info;
	// regulationsion graph data - for each regulating component hold the values in the order of the thresholds
	map<CompID, vector<double> > reg_corr;
	map<CompID, vector<double> > reg_freq;
	map<CompID, vector<char> > reg_sign;
	map<CompID, double> expected_freq; // Expected frequency is shared for all the edges
	RegData(const RegInfo & info) : _info(info) 
	{
		for (const auto & regulator : _info.regulators) {
			reg_corr.insert({ regulator.first, vector<double>(regulator.second.size(), 0.0) });
			reg_freq.insert({ regulator.first, vector<double>(regulator.second.size(), 0.0) });
			reg_sign.insert({ regulator.first, vector<char>(regulator.second.size(), 0) });
			expected_freq.insert({ regulator.first, 0.0 });
		}
	}
};
using RegsData = vector<RegData>;

