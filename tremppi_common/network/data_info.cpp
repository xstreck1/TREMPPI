#include "data_info.hpp"
#include "../general/data_conv.hpp"

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


vector<string> DataInfo::getAllContexts(const RegInfos & reg_infos) {
	vector<string> result;
	for (const RegInfo & reg_info : reg_infos)
		for (const auto & column : reg_info.columns)
			result.push_back(column.second);
	return result;
}


CompID DataInfo::getCompID(const RegInfos & reg_infos, const string & name) {
	auto reg_info_it = find_if(WHOLE(reg_infos), [&name](const RegInfo & reg_info){ return reg_info.name == name;  });
	if (reg_info_it == end(reg_infos)) {
		throw runtime_error("Component " + name + " not found");
		return INF;
	}
	else {
		return reg_info_it->ID;
	}
}

CompID DataInfo::getCompID(const map<string, CompID> & components, const string & name) {
	if (components.count(name) < 1)
		throw runtime_error("Component " + name + " not found");
	else
		return components.at(name);
}

map<CompID, vector<vector<size_t>>> DataInfo::getColumnsOfThresholds(const RegInfo & reg_info)
{
	map<CompID, vector<vector<size_t>>> result;

	for (auto & regulator : reg_info.regulators) {
		vector<vector<size_t> > columns(regulator.second.size() + 1);
		const size_t reg_i = getRegulatorI(regulator.first, reg_info);
		map<ActLevel, size_t> threshold_to_index = DataConv::getThresholdToIndex(regulator.second);

		// Distribute the column indices based on the value of the threshold in the specific column
		for (auto & column : reg_info.columns) {
			auto & trhs = DataConv::getThrsFromContext(column.second);
			ActLevel trh = trhs[reg_i];
			size_t i = threshold_to_index[trh];
			columns[i].push_back(column.first);
		}

		result.insert({ regulator.first, move(columns) });
	}

	return result;
}

size_t DataInfo::getRegulatorI(const CompID ID, const RegInfo & reg_info)
{
	return distance(begin(reg_info.regulators), reg_info.regulators.find(ID));
}
