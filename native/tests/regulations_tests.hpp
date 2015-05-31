#pragma once

#include "common_tests_data.hpp"

int basic_regulations_test() {
	vector<string> arguments = { "--path", bfs::absolute(TremppiSystem::HOME_PATH / TEST_FOLDER / TEST_PROJECT).string() };
	ArgPtr arg_ptr(arguments);
	return (tremppi_regulations(arg_ptr.getArgc(), arg_ptr.getArgv()));
}
