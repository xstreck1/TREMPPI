#include "header.h"

void TremppiSystem::set(const char * name, const char * arg, const bfs::path & _WORK_PATH) {
	PROGRAM_NAME = name;
	WORK_PATH = _WORK_PATH;

	// Set the home
	char* home_path = getenv("TREMPPI_HOME");

	if (home_path != NULL)
		HOME_PATH = bfs::path{ home_path };
	else {
		bfs::path test_path{ arg };

		// Test for ../data to exist
		test_path = test_path.parent_path() /= bfs::path{ ".." };
		if (bfs::exists(test_path / bfs::path{ "data" }))
			HOME_PATH = test_path;
		// Test for ../../data to exist - Windows only
		else {
			test_path = test_path /= bfs::path{ ".." };
			if (bfs::exists(test_path / bfs::path{ "data" }))
				HOME_PATH = test_path;
		}
	}
}

// Global variable
TremppiSystem tremppi_system;