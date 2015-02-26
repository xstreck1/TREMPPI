#pragma once

#include <json/json.h>

#define IF_CMP if(out["setup"]["comparative"].asBool())

namespace Report {
	Json::Value createSetup();
}