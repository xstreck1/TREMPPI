#pragma once

#include <tremppi_common/network/data_info.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Class that outputs formatted resulting data.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OutputManager 
{
	const bool witness; ///< Write the witness?
	const bool robustness; ///< Write the robustness?
	const RegInfos & reg_infos; ///< Reference to the model itself.
	const string & name; ///< Name of the property
	sqlite3pp::database & db;
	map<string, string> new_columns; ///< (column name, column type)

public:
	NO_COPY(OutputManager);

	// Store the names of the columns to be used
	OutputManager(const bool _witness, const bool _robustness, const RegInfos & _reg_infos, const string & _name, sqlite3pp::database & _db);

	// Write the new columns to the database
	void outputForm();

	// Output parametrizations from this round together with additional data, if requested.
	void outputRound(const size_t cost, const double robustness_val, const string & witness_path, const Levels & parametrization, const ParamNo & rowid);
};