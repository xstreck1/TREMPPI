#pragma once
#include "common_functions.hpp"

namespace DataInfo {
	// Controls if the names is formed as [a-zA-Z][a-zA-Z0-9_]*
	bool isValidSpecName(const string & spec_name) {
		if (spec_name.empty())
			return false;
		bool valid = isalpha(spec_name[0]) || spec_name[0] == '_';
		for (char ch : spec_name)
			valid &= isalnum(ch) || ch == '_';
		return valid;
	}
}