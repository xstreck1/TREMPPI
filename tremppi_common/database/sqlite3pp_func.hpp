#pragma once

#include <sqlite3pp/sqlite3ppext.hpp>
#include "../header.h"

namespace sqlite3pp {
	namespace func {
		// @return	a dictionary with keys being the columns that match the expression and values are their indeces
		map<string, size_t> matchingColumns(const string & table_name, const regex & control_expression, database & db) {
			map<string, size_t> result;

			sqlite3pp::query qry(db, ("SELECT * FROM " + table_name).c_str());
			for (int i = 0; i < qry.column_count(); ++i)
				if (regex_match(qry.column_name(i), control_expression))
					result.insert(make_pair(qry.column_name(i), i));

			return result;
		}

		// @return  true if a colum of the given name exists
		bool hasColumn(const string & table_name, const string & column_name, sqlite3pp::database & db) {
			sqlite3pp::query qry(db, ("SELECT * FROM " + table_name).c_str());
			for (int column_i = 0; column_i < qry.column_count(); column_i++) 
				if (qry.column_name(column_i) == column_name)
					return true;
			return false;
		}

		// Add a row to the table if it is not present or null its values if it is.
		void addColumn(const string & table_name, const string & column_name, const string & type, sqlite3pp::database & db) {
			if (!hasColumn(table_name, column_name, db))
				db.execute(("ALTER TABLE " + table_name + " ADD " + column_name + " " + type + ";").c_str());
			else
				db.execute(("UPDATE " + table_name + " SET " + column_name + "=NULL;").c_str());
		}

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

using namespace sqlite3pp;