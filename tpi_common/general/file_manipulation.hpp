#pragma once

#include "../header.h"
#include "common_functions.hpp"

namespace FileManipulation {
	void copyDir(bfs::path const & source, bfs::path const & destination)
	{
		// Check whether the function call is valid
		if (!bfs::exists(source) || !bfs::is_directory(source))	{
			throw runtime_error("Source directory " + source.string() + " does not exist or is not a directory.");
		}
		if (bfs::exists(destination)) {
			throw runtime_error("Destination directory " + destination.string() + " already exists.");
		}
		// Create the destination directory
		if (!bfs::create_directory(destination)) {
			throw runtime_error("Unable to create destination directory" + destination.string());
		}
		// Iterate through the source directory
		for (bfs::directory_iterator file(source); file != bfs::directory_iterator(); ++file) {
			bfs::path current(file->path());
			if (bfs::is_directory(current)) {
				// Found directory: Recursion
				copyDir(current, destination / current.filename());
			}
			else {
				// Found file: Copy
				bfs::copy_file(current, destination / current.filename());
			}
		}
	}
}