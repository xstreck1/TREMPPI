#include "data_conv.hpp"

pair<size_t, size_t> DataConv::indices2range(const map<string, size_t> & columns) {
	pair<size_t, size_t> result;

	result.first = min_element(WHOLE(columns), columns.value_comp())->second;
	result.second = max_element(WHOLE(columns), columns.value_comp())->second;
	if (result.second - result.first + 1 != columns.size())
		throw runtime_error("columns do not form a range");

	return result;
}

vector<string> DataConv::columns2list(const map<string, size_t> & columns) {
	vector<string> columns_list(columns.size());
	transform(WHOLE(columns), begin(columns_list), [](const pair<string, size_t> column) {return column.first; });
	return columns_list;
}