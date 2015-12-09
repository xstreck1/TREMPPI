#pragma once

#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/network/definitions.hpp>

namespace WitnessOutput 
{
	inline string remStep(const string & state);

	inline string getStep(const string & state);

	Json::Value convert(const map<pair<string, string>, size_t> & transitions, ParamNo max_count);
}