#include "header.h"

void TremppiSystem::set(const char * name, const char * arg, const string & _WORK_PATH) {
	PROGRAM_NAME = name;
	WORK_PATH = _WORK_PATH;
	BIN_PATH = bfs::path{ arg };
	EXEC_PATH = bfs::current_path();

	// Set the home
	char* home_path = getenv("TREMPPI_HOME");

	if (home_path != NULL)
		HOME_PATH = bfs::path{ home_path };
	else {
		// Test for ../tremppi to exist
		bfs::path test_path = BIN_PATH / bfs::path{ ".." };
		if (bfs::exists(test_path / bfs::path{ "tremppi" }))
			HOME_PATH = test_path;
		// Test for ../../tremppi to exist - Windows only
		else {
			test_path = test_path /= bfs::path{ ".." };
			if (bfs::exists(test_path / bfs::path{ "tremppi" }))
				HOME_PATH = test_path;
		}
	}
}

// Global variable
TremppiSystem tremppi_system;