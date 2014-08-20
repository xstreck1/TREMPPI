#pragma once

#include "../network/definitions.hpp"
#include "../general/data_conv.hpp"
#include "sqlite3pp_func.hpp"

// Regulatory graph info, stored by a component
struct RegInfo {
	string name;
	ActLevel max;
	map<string, size_t> columns;
	pair<size_t, size_t> columns_range;
	map<string, Levels> regulators;
};
using RegInfos = vector<RegInfo>;

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

	//
	Levels getThrFromContexts(const vector<string> & columns_name, const size_t regul_i);

	//
	Levels getThrsFromContext(const string & column_name); 

	// Read headers for all the regulatory functions
	RegInfo readRegInfo(const string & name, sqlite3pp::database & db);

	sqlite3pp::query selectionFilter(const map<string, size_t> & columns, const string & selection, sqlite3pp::database & db);

	sqlite3pp::query selectionIDs(const string & selection, sqlite3pp::database & db);
};