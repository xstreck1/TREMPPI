#pragma once

#include <boost/python/detail/wrap_python.hpp>

#include <tremppi_common/python/python_functions.hpp>

#include "common_tests_data.hpp"

int basic_init_test() {
	vector<string> arguments = { "--path", bfs::absolute(bfs::path{ TremppiSystem::HOME_PATH } / "test" ).string(), "test_proj" };
	ArgPtr arg_ptr(arguments);
	return (tremppi_init(arg_ptr.getArgc(), arg_ptr.getArgv()));
}

int basic_update_test() {
	vector<string> arguments = { "--path", bfs::absolute(bfs::path{ TremppiSystem::HOME_PATH } / "test" / "test_proj").string() };
	ArgPtr arg_ptr(arguments);
	return (tremppi_update(arg_ptr.getArgc(), arg_ptr.getArgv()));
}