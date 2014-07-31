#pragma once

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

	//
	void copyReport(const bfs::path & input_path) {
		// Copy the data from "../Files/Standine_report"
		bfs::path report_dir = input_path.parent_path() /= (input_path.stem().string() + "_Standinet");
		if (bfs::exists(report_dir))
			bfs::remove_all(report_dir);
		copyDir(bfs::path{ "../Files/Standinet_report" }, report_dir);

		// Delete the original models
		vector<string> to_delete = { "data.js", "configure.json", "regulatory_select.json", "regulatory_differ.json", "regulatory_compare.json" };
		for_each(WHOLE(to_delete), [&report_dir](const string & filename) {
			bfs::remove(report_dir /= bfs::path{ filename });
		});
	}

	//
	ofstream fileOutput(const bfs::path & input_path, const string & name) {
		bfs::path output_path = input_path.parent_path() /= bfs::path{ input_path.stem().string() + "_Standinet" } /= name;
		ofstream file = ofstream(output_path.string(), ios::out);
		if (!file)
			throw runtime_error("Could not open " + output_path.string());
		return file;
	}
}