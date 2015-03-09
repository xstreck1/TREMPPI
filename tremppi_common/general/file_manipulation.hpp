#pragma once

#include <json/json.h>

#include "../header.h"
#include "common_functions.hpp"

namespace FileManipulation {
	//
	void copyDir(bfs::path const & source, bfs::path const & destination);

	//
	Json::Value parseJSON(const bfs::path & input_path);

	//
	void writeJSON(const bfs::path & output_path, const Json::Value & data);

	//
	void replaceInFile(bfs::path const & file, const string & original, const string & replacement);

	//
	void copyAnalysisFiles(const bfs::path & path, const string & name);
}
