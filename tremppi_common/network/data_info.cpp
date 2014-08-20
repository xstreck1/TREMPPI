#include "data_info.hpp"

bool DataInfo::isValidSpecName(const string & spec_name) {
	if (spec_name.empty())
		return false;
	bool valid = isalpha(spec_name[0]) || spec_name[0] == '_';
	for (char ch : spec_name)
		valid &= isalnum(ch) || ch == '_';
	return valid;
}

map<string, ActLevel> DataInfo::getComponents(const Json::Value & nodes) {
	map<string, ActLevel>components;

	for (const Json::Value node : nodes)
		components.insert(make_pair(node["data"]["id"].asString(), node["data"]["MaxActivity"].asInt()));

	return components;
}