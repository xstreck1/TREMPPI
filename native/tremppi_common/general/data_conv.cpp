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

#include "data_conv.hpp"
pair<size_t, size_t> DataConv::indices2range(const map<size_t, string> & columns) 
{
	pair<size_t, size_t> result;

	result.first = min_element(WHOLE(columns))->first;
	result.second = max_element(WHOLE(columns))->first;
	if (result.second - result.first + 1 != columns.size())
		throw runtime_error("columns do not form a range");

	return result;
}
vector<string> DataConv::columns2list(const map<size_t, string> & columns) 
{
	vector<string> columns_list(columns.size());
	transform(WHOLE(columns), begin(columns_list), [](const pair<size_t, string>  column) {return column.second; });
	return columns_list;
}

Levels DataConv::getThrFromContexts(const vector<string> & columns_name, const size_t regul_i) 
{
	Levels result;

	for (const size_t context_i : cscope(columns_name)) 
	{
		string digits = getAllMatches("_(\\d*)\\b", columns_name[context_i], 1).front();
		result.emplace_back(lexical_cast<ActLevel>(digits[regul_i]));
	}

	return result;
}

Levels DataConv::getThrsFromContext(const string & column_name) 
{
	Levels result;

	string digits = getAllMatches("_(\\d*)\\b", column_name, 1).front();
	result.resize(digits.size());
	transform(WHOLE(digits), begin(result), [](char digit ) { return lexical_cast<ActLevel>(digit);} );

	return result;
}
map<ActLevel, size_t> DataConv::getThresholdToIndex(const Levels & thresholds) 
{
	map<ActLevel, size_t> result;

	result.insert({ 0, 0 });
	for (const ActLevel & threshold : thresholds) 
	{
		result.insert({ threshold, result.size() });
	}

	return result;
}