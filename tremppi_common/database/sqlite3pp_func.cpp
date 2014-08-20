#include "sqlite3pp_func.hpp"

map<string, size_t> sqlite3pp::func::matchingColumns(const string & table_name, const regex & control_expression, database & db) {
	map<string, size_t> result;

	sqlite3pp::query qry(db, ("SELECT * FROM " + table_name).c_str());
	for (int i = 0; i < qry.column_count(); ++i)
		if (regex_match(qry.column_name(i), control_expression))
			result.insert(make_pair(qry.column_name(i), i));

	return result;
}

bool sqlite3pp::func::hasColumn(const string & table_name, const string & column_name, sqlite3pp::database & db) {
	sqlite3pp::query qry(db, ("SELECT * FROM " + table_name).c_str());
	for (int column_i = 0; column_i < qry.column_count(); column_i++)
		if (qry.column_name(column_i) == column_name)
			return true;
	return false;
}

void sqlite3pp::func::addColumn(const string & table_name, const string & column_name, const string & type, sqlite3pp::database & db) {
	if (!hasColumn(table_name, column_name, db))
		db.execute(("ALTER TABLE " + table_name + " ADD " + column_name + " " + type + ";").c_str());
	else
		db.execute(("UPDATE " + table_name + " SET " + column_name + "=NULL;").c_str());
}