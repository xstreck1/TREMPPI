#pragma once

#include "../network/definitions.hpp"
#include "../network/data_info.hpp"
#include "../general/data_conv.hpp"
#include "sqlite3pp_func.hpp"

/// Reads parametrizations form given file
class DatabaseReader {
	const string NAMES_COLUMN = "Name";
	const string MAX_LEVEL_COLUMN = "MaxActivity";
	const string REGULATOR_COLUMN = "Source";
	const string TARGET_COLUMN = "Target";
	const string THRESHOLD_COLUMN = "Threshold";
	map<string, CompID> components_dict;
	vector<string> components;
	map<string, ActLevel> maxes_dict;
	vector<ActLevel> maxes;

	// Obtain components and their ids
	void readComponents(sqlite3pp::database & db);

	// Obtain maximal levels 
    void readMaxes(sqlite3pp::database & db);

	//
	vector<Levels> obtainRequirements(const string & context, const map<CompID, Levels> & regulators, sqlite3pp::database & db);
	
	// Get regulators of the given component
	map<CompID, Levels> readRegulators(const string & name, sqlite3pp::database & db);

public:
	// Read headers for all the regulatory functions
	RegInfos readRegInfos(sqlite3pp::database & db);

	// 
	static sqlite3pp::query selectionFilter(const map<size_t, string> & columns, const string & selection, sqlite3pp::database & db);

	//
	static sqlite3pp::query selectionIDs(const string & selection, sqlite3pp::database & db);
};
