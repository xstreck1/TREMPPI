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

#pragma once

#include <sqlite3pp/sqlite3ppext.hpp>
#include "../general/common_functions.hpp"
#include "../header.h"

namespace sqlite3pp 
{
	namespace func {
		// @return	a dictionary with keys being the columns that match the expression and values are their indeces
		map<size_t, string> matchingColumns(const string & table_name, const regex & control_expression, database & db);

		// @return  true if a colum of the given name exists
		bool hasColumn(const string & table_name, const string & column_name, sqlite3pp::database & db);

		// Add a row to the table if it is not present or null its values if it is.
		void addColumn(const string & table_name, const string & column_name, const string & type, sqlite3pp::database & db);

		// Obtain number of columns in selection
		size_t rowCount(const string & table_name, const string & selection, sqlite3pp::database & db);;
		size_t rowCount(const string & table_name, sqlite3pp::database & db);

		// @return data references by indices specified in the columns vecto
		template <typename DataType>
		vector<DataType> getRow(const sqlite3pp::query::rows & row, const size_t first_column, const size_t column_count) 
		{
			vector<DataType> result;

			for (const size_t column_i : crange(first_column, first_column + column_count))
				result.push_back(row.get<DataType>(column_i));

			return result;
		}
	}
}
