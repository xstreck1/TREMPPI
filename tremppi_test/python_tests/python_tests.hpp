#pragma once

#include <gtest/gtest.h>

#include <boost/python/detail/wrap_python.hpp>
#include <tremppi_common/general/system.hpp>

#include "../common_tests/common_tests_data.hpp"

int tremppi_init(int, char**);
int tremppi_update(int, char**);

int basic_init_test() {
	vector<string> arguments = { "--path", bfs::absolute(bfs::path{ tremppi_system.HOME_PATH } / "test" ).string(), "test_proj" };
	ArgPtr arg_ptr(arguments);
	return (tremppi_init(arg_ptr.getArgc(), arg_ptr.getArgv()));
}

int basic_update_test() {
	vector<string> arguments = { "--path", bfs::absolute(bfs::path{ tremppi_system.HOME_PATH } / "test" / "test_proj").string() };
	ArgPtr arg_ptr(arguments);
	return (tremppi_update(arg_ptr.getArgc(), arg_ptr.getArgv()));
}

