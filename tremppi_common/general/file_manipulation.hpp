#pragma once

#include <json/json.h>

#include "../header.h"
#include "common_functions.hpp"

namespace FileManipulation {
	//
	void copyDir(bfs::path const & source, bfs::path const & destination);

	//
	Json::Value readJSasJSON(const bfs::path & input_path);
}