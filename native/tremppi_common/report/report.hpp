#pragma once

#include <json/json.h>

#include "../network/data_info.hpp"
namespace Report 
{
	Json::Value createSetup();
	string reformName(const RegInfos & reg_infos, const string & name);
	string contextToValues(const RegInfos & reg_infos, const string & context);
}