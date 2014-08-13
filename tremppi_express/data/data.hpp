#pragma once

#include <tremppi_common/general/common_functions.hpp>

// Common to all the sets of regulatory functions
struct RegFunc {
	// Read from the database
	CompID ID;
	string name;
	ActLevel max;
	map<string, Levels > regulators;
	vector<string> columns_name;
	vector<size_t> columns_ID;
	vector<Levels> columns_minterm;
};
using RegFuncs = vector<RegFunc>;

// Post Algebra types
using PVal = int;
using PLit = vector<PVal>; ///< Post's algebra literal
using PMin = vector<PLit>; ///< Post's algebra minterm
using PDNF = vector<PMin>; ///< Post's algebra disjunctive normal form