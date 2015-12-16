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

#include "sqlite3pp_func.hpp"

map<size_t, string> sqlite3pp::func::matchingColumns(const string & table_name, const regex & control_expression, database & db)
{
	map<size_t, string> result;

	sqlite3pp::query qry(db, ("SELECT * FROM " + table_name).c_str());
	for (int i = 0; i < qry.column_count(); ++i)
		if (regex_match(qry.column_name(i), control_expression))
			result.insert(make_pair(i, qry.column_name(i)));

	return result;
}

bool sqlite3pp::func::hasColumn(const string & table_name, const string & column_name, sqlite3pp::database & db)
{
	sqlite3pp::query qry(db, ("SELECT * FROM " + table_name).c_str());
	for (int column_i = 0; column_i < qry.column_count(); column_i++)
		if (qry.column_name(column_i) == column_name)
			return true;
	return false;
}

void sqlite3pp::func::addColumn(const string & table_name, const string & column_name, const string & type, sqlite3pp::database & db)
{
	if (std::count_if(WHOLE(column_name), [](const char ch) {return isspace(ch);})) {
		throw runtime_error("Whitespace not allowed for table column in the name '" + column_name + "'");
	}
	if (!hasColumn(table_name, column_name, db))
		db.execute(("ALTER TABLE " + table_name + " ADD " + column_name + " " + type + ";").c_str());
	/*else
		db.execute(("UPDATE " + table_name + " SET " + column_name + "=NULL;").c_str());*/
}

size_t sqlite3pp::func::rowCount(const string & table_name, const string & selection, sqlite3pp::database & db)
{
	sqlite3pp::query qry(db, ("SELECT COUNT(*) FROM " + table_name + " WHERE " + selection).c_str());
	return qry.begin()->get<size_t>(0);
}

size_t sqlite3pp::func::rowCount(const string & table_name, sqlite3pp::database & db)
{
	return rowCount(table_name, "1", db);
}