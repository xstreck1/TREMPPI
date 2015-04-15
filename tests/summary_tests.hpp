#pragma once

#include "common_tests_data.hpp"

int basic_summary_test() {
	vector<string> arguments = { "--path", bfs::absolute(bfs::path{ TremppiSystem::HOME_PATH } / "test" / "test_proj").string() };
	ArgPtr arg_ptr(arguments);
	return (tremppi_summary(arg_ptr.getArgc(), arg_ptr.getArgv()));
}
