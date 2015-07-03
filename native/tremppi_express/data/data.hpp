#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/database/database_reader.hpp>

// Common to all the sets of regulatory functions

struct RegFunc 
{
	const RegInfo info;
	Configurations minterms;
};
using RegFuncs = vector<RegFunc>;

// Post Algebra types
using PVal = int;
using PLit = vector<PVal>; ///< Post's algebra literal
using PMin = vector<PLit>; ///< Post's algebra minterm
using PDNF = vector<PMin>; ///< Post's algebra disjunctive normal form