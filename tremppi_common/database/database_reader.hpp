#pragma once

#include "../network/definitions.hpp"
#include "../network/data_info.hpp"
#include "../general/data_conv.hpp"
#include "sqlite3pp_func.hpp"

/// Reads parametrizations form given file
namespace DatabaseReader {
	const string NAMES_COLUMN = "Name";
	const string MAX_LEVEL_COLUMN = "MaxActivity";
	const string REGULATOR_COLUMN = "Source";
	const string TARGET_COLUMN = "Target";
	const string THRESHOLD_COLUMN = "Threshold";

	// Obtain maximal levels for the individual components
	ActLevel getMaxLevel(const string & name, sqlite3pp::database & db);

	// Get regulators of the given component
	map<string, Levels> obtainRegulators(const string & component, sqlite3pp::database & db);

	// Read headers for all the regulatory functions
	RegInfo readRegInfo(const CompID ID, const string & name, sqlite3pp::database & db);

	// 
	sqlite3pp::query selectionFilter(const map<size_t, string> & columns, const string & selection, sqlite3pp::database & db);

	//
	sqlite3pp::query selectionIDs(const string & selection, sqlite3pp::database & db);

	//
	map<string, Levels> obtainRequirements(const string & context, const map<string, Levels> & regulators, sqlite3pp::database & db);
};
