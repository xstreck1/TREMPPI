#include "system.hpp"

// Define variables
string TremppiSystem::PROGRAM_NAME;
bfs::path TremppiSystem::HOME_PATH;
bfs::path TremppiSystem::WORK_PATH;
bfs::path TremppiSystem::EXEC_PATH;
bfs::path TremppiSystem::BIN_PATH;
bfs::path TremppiSystem::DATA_PATH;
bool TremppiSystem::standalone;

void TremppiSystem::set(const char * name, const char * arg, const string & _WORK_PATH) 
{
	TremppiSystem::PROGRAM_NAME = name;
	TremppiSystem::WORK_PATH = _WORK_PATH;
	TremppiSystem::DATA_PATH = TremppiSystem::WORK_PATH / DATA_FOLDER;
	TremppiSystem::BIN_PATH = bfs::absolute(bfs::path{ arg });
	TremppiSystem::EXEC_PATH = bfs::current_path();

	// Set the home
	char* home_path = getenv("TREMPPI_HOME");
	bfs::path test_path = BIN_PATH;

	if (home_path != NULL)
		TremppiSystem::HOME_PATH = bfs::path{ home_path };
	// Descend three steps
	else if (bfs::exists((test_path = test_path.parent_path()) / bfs::path{ "tremppi" }))
		TremppiSystem::HOME_PATH = test_path;
	else if (bfs::exists((test_path = test_path.parent_path()) / bfs::path{ "tremppi" }))
		TremppiSystem::HOME_PATH = test_path;
	else if (bfs::exists((test_path = test_path.parent_path()) / bfs::path{ "tremppi" }))
		TremppiSystem::HOME_PATH = test_path;
	else 
	{
		throw runtime_error("Binary " + TremppiSystem::BIN_PATH.string() + "is not homed and TREMPPI_HOME not found. Either set up the system variable of the same name pointing to the TREMMPI folder, or put the binary in the TREMPPI/bin folder.");
	}
}