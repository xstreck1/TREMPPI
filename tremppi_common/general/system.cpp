#include "system.hpp"

void TremppiSystem::set(const char * name, const char * arg, const string & _WORK_PATH) {
	PROGRAM_NAME = name;
	WORK_PATH = _WORK_PATH;
	BIN_PATH = bfs::absolute(bfs::path{ arg });
	EXEC_PATH = bfs::current_path();

	// Set the home
	char* home_path = getenv("TREMPPI_HOME");
	bfs::path test_path = BIN_PATH;

	if (home_path != NULL)
		HOME_PATH = bfs::path{ home_path };
	// Descend three steps
	else if (bfs::exists((test_path = test_path.parent_path()) / bfs::path{ "tremppi" }))
		HOME_PATH = test_path;
	else if (bfs::exists((test_path = test_path.parent_path()) / bfs::path{ "tremppi" }))
		HOME_PATH = test_path;
	else if (bfs::exists((test_path = test_path.parent_path()) / bfs::path{ "tremppi" }))
		HOME_PATH = test_path;
	else {
		throw runtime_error("Binary " + BIN_PATH.string() + "is not homed and TREMPPI_HOME not found. Either set up the system variable of the same name pointing to the TREMMPI folder, or put the binary in the TREMPPI/bin folder.");
	}
}

// Global variable
TremppiSystem tremppi_system;