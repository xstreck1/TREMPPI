#pragma once

#include "../header.h"
#include "common_functions.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Common things about parsing program options for TREMPPI
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ProgramOptions {
protected:
	bpo::options_description visible, invisible, all;
	bpo::positional_options_description pos_decr;
public:
	GROUNDED(ProgramOptions)
	//
	virtual bpo::variables_map parseProgramOptions(int argc, char ** argv);
	// Obtain the path to the input file
	static bfs::path getPath(const bpo::variables_map & po, const string & filename);
	//
	static string ProgramOptions::getFilter(const bfs::path & filter_path);
};