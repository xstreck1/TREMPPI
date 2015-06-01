#pragma once

#include "common_tests_data.hpp"

int basic_label_test() {
	vector<string> arguments = { "--path", bfs::absolute(TremppiSystem::HOME_PATH / TEST_FOLDER / TEST_PROJECT).string() };
	ArgPtr arg_ptr(arguments);
	return (tremppi_label(arg_ptr.getArgc(), arg_ptr.getArgv()));
}

