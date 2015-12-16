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

#include <tremppi_common/network/data_info.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Class that outputs formatted resulting data.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OutputManager 
{
	const bool cost; ///< Write the cost?
	const bool trace; ///< Write the trace?
	const bool robustness; ///< Write the robustness?
	const RegInfos & reg_infos; ///< Reference to the model itself.
	const string & name; ///< Name of the property
	sqlite3pp::database & db;
	map<string, string> new_columns; ///< (column name, column type)

public:
	NO_COPY(OutputManager);

	// Store the names of the columns to be used
	OutputManager(const bool _cost, const bool _witness, const bool _robustness, const RegInfos & _reg_infos, const string & _name, sqlite3pp::database & _db);

	// Write the new columns to the database
	void outputForm();

	// Output parametrizations from this round together with additional data, if requested.
	void outputRound(const size_t cost, const double robustness_val, const string & witness_path, const Levels & parametrization, const ParamNo & rowid);
};