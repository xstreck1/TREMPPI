#pragma once

#include "../header.h"
#include "common_functions.hpp"

namespace FileManipulation {
	//
	void copyDir(bfs::path const & source, bfs::path const & destination);
}