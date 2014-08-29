#include "data_info.hpp"

bool DataInfo::isValidName(const string & spec_name) {
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

vector<string> DataInfo::getAllNames(const RegInfos & reg_infos) {
	vector<string> names;
	for (const RegInfo & reg_info : reg_infos)
		names.push_back(reg_info.name);
	return names;
}

ActLevel DataInfo::getMaxLevel(const RegInfos & reg_infos) {
	return rng::max_element(reg_infos, [](const RegInfo & A, const RegInfo & B) {
		return A.max_activity < B.max_activity;
	})->max_activity;
}