#pragma once

#include <sqlite3pp/sqlite3ppext.hpp>
#include "../header.h"

namespace sqlite3pp {
	namespace func {
		// @return	a dictionary with keys being the columns that match the expression and values are their indeces
		map<size_t, string> matchingColumns(const string & table_name, const regex & control_expression, database & db);

		// @return  true if a colum of the given name exists
		bool hasColumn(const string & table_name, const string & column_name, sqlite3pp::database & db);

		// Add a row to the table if it is not present or null its values if it is.
		void addColumn(const string & table_name, const string & column_name, const string & type, sqlite3pp::database & db);

		// @return data references by indices specified in the columns vector
		template <typename DataType>
		vector<DataType> getRow(const sqlite3pp::query::iterator & i, const size_t column_count) {
			vector<DataType> data;

			for (const size_t column_i : crange(column_count)) {
				data.push_back((*i).get<DataType>(column_i));
			}

			return data;
		}
	}
}