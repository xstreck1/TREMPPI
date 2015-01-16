#pragma once

#include <gtest/gtest.h>

#include <tremppi_common/header.h>

#include "../common_tests/common_tests_data.hpp"

int tremppi_express(int, char**);

int basic_express_test() {
	vector<string> arguments = { "--path", bfs::absolute(bfs::path{ TremppiSystem::HOME_PATH } / "test" / "test_proj").string() };
	ArgPtr arg_ptr(arguments);
	return (tremppi_express(arg_ptr.getArgc(), arg_ptr.getArgv()));
}

