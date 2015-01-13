#pragma once

#include <gtest/gtest.h>
#include <tremppi_common/header.h>

#include "../common_tests/common_tests_data.hpp"

int tremppi_report(int, char**);

int basic_report_test() {
	vector<string> arguments = { "--path", bfs::absolute(bfs::path{ tremppi_system.HOME_PATH } / "test" / "test_proj").string(), "-r" };
	ArgPtr arg_ptr(arguments);
	return (tremppi_report(arg_ptr.getArgc(), arg_ptr.getArgv()));
}
