#include "data_conv.hpp"

pair<size_t, size_t> DataConv::indices2range(const map<size_t, string> & columns) {
	pair<size_t, size_t> result;

	result.first = min_element(WHOLE(columns))->first;
	result.second = max_element(WHOLE(columns))->first;
	if (result.second - result.first + 1 != columns.size())
		throw runtime_error("columns do not form a range");

	return result;
}

vector<string> DataConv::columns2list(const map<size_t, string> & columns) {
	vector<string> columns_list(columns.size());
	transform(WHOLE(columns), begin(columns_list), [](const pair<size_t, string>  column) {return column.second; });
	return columns_list;
}