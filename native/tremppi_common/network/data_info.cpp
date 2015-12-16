/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "data_info.hpp"
#include "../general/data_conv.hpp"

bool DataInfo::isValidName(const string & spec_name) 
{
	if (spec_name.empty())
		return false;
	bool valid = isalpha(spec_name[0]);
	for (char ch : spec_name)
		valid = valid && isalnum(ch);
	return valid;
}

map<string, ActLevel> DataInfo::getComponents(const Json::Value & nodes) 
{
	map<string, ActLevel>components;

	for (const Json::Value node : nodes)
		components.insert(make_pair(node["data"]["id"].asString(), node["data"]["MaxActivity"].asInt()));

	return components;
}
vector<string> DataInfo::getAllNames(const RegInfos & reg_infos) 
{
	vector<string> names;
	for (const RegInfo & reg_info : reg_infos)
		names.push_back(reg_info.name);
	return names;
}
ActLevel DataInfo::getMaxLevel(const RegInfos & reg_infos) 
{
	return rng::max_element(reg_infos, [](const RegInfo & A, const RegInfo & B) {
		return A.max_activity < B.max_activity;
	})->max_activity;
}
vector<string> DataInfo::getAllContexts(const RegInfos & reg_infos) 
{
	vector<string> result;
	for (const RegInfo & reg_info : reg_infos)
		for (const auto & column : reg_info.columns)
			result.push_back(column.second);
	return result;
}

CompID DataInfo::getCompID(const RegInfos & reg_infos, const string & name) 
{
	auto reg_info_it = find_if(WHOLE(reg_infos), [&name](const RegInfo & reg_info){ return reg_info.name == name;  });
	if (reg_info_it == end(reg_infos)) 
	{
		throw runtime_error("Component " + name + " not found");
		return INF;
	}
	else 
	{
		return reg_info_it->ID;
	}
}
CompID DataInfo::getCompID(const map<string, CompID> & components, const string & name) 
{
	if (components.count(name) < 1u)
		throw runtime_error("Component " + name + " not found");
	else
		return components.at(name);
}

size_t DataInfo::RegIDToRegNo(const RegInfo & reg_info, const CompID reg_ID) 
{
	return distance(begin(reg_info.regulators), reg_info.regulators.find(reg_ID));
}

CompID DataInfo::RegNoToRegID(const RegInfo & reg_info, const size_t reg_no) 
{
	auto it = begin(reg_info.regulators);
	for (size_t i = 0; i < reg_no; i++) 
	{
		it++;
	}
	return it->first;
}

// 
size_t DataInfo::columnNoToColumnID(const RegInfo & reg_info, const size_t column_no) 
{
	auto it = begin(reg_info.columns);
	for (int i = 0; i < column_no; i++) 
	{
		it++;
	}
	return it->first;
}

//
size_t DataInfo::columnIDToColumnNo(const RegInfo & reg_info, const size_t column_ID) 
{
	return distance(begin(reg_info.columns), reg_info.columns.find(column_ID));
}

size_t DataInfo::getColumnWithout(const RegInfo & reg_info, const size_t column_no, const CompID reg_ID, const ActLevel threshold) 
{
	const auto & thresholds = reg_info.regulators.at(reg_ID);
	const auto threshold_it = find(WHOLE(thresholds), threshold);
	const ActLevel lower_threshold = (threshold_it == begin(thresholds)) ? 0 : *(threshold_it - 1);
	Levels new_context = reg_info.contexts.at(column_no);
	const size_t reg_no = RegIDToRegNo(reg_info, reg_ID);
	new_context[reg_no] = lower_threshold;
	for (const auto & contex : reg_info.contexts) 
	{
		if (contex.second == new_context) {
			return contex.first;
		}
	}
	throw runtime_error("Did not find a context after removal of a regulator from the component " + reg_info.name);
}

//
vector<string> DataInfo::columnsVector(const RegInfo & reg_info) 
{
	vector<string> result;
	for (const auto & column : reg_info.columns) 
	{
		result.emplace_back(column.second);
	}
	return result;
}

//
vector<Levels> DataInfo::contextsVector(const RegInfo & reg_info) 
{
	vector<Levels> result;
	for (const auto & column : reg_info.contexts) 
	{
		result.emplace_back(column.second);
	}
	return result;
}