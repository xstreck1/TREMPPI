#pragma once

#include <json/json.h>

namespace Report 
{
	Json::Value createSetup();
	std::string reformName(std::string name);
}