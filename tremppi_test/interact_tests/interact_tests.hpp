#pragma once

#include <gtest/gtest.h>
#include <tremppi_common/header.h>

#include "../common_tests/common_tests_data.hpp"

int tremppi_interact(int, char**);

int basic_interact_test() {
	vector<string> arguments = { "--path", bfs::absolute(bfs::path{ TremppiSystem::HOME_PATH } / "test" / "test_proj").string() };
	ArgPtr arg_ptr(arguments);
	return (tremppi_interact(arg_ptr.getArgc(), arg_ptr.getArgv()));
}
