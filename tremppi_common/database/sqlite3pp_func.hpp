#include <sqlite3pp/sqlite3ppext.hpp>
#include "../header.h"

#pragma once

namespace sqlite3pp {
	namespace func {
		map<string, size_t> matchingColumns(const string & table_name, const regex & control_expression, database & db) {
			map<string, size_t> result;

			sqlite3pp::query qry(db, ("SELECT * FROM " + table_name).c_str());
			for (int i = 0; i < qry.column_count(); ++i)
				if (regex_match(qry.column_name(i), control_expression))
					result.insert(make_pair(qry.column_name(i), i));

			return result;
		}
	}
}

using namespace sqlite3pp;