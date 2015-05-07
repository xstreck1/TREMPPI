#pragma once

#include <boost/python/detail/wrap_python.hpp>

#include <tremppi_common/python/python_functions.hpp>

#include "common_tests_data.hpp"

int basic_init_test() {
	vector<string> arguments = { "--path", bfs::absolute(TremppiSystem::HOME_PATH / TEST_FOLDER).string(), TEST_PROJECT };
	ArgPtr arg_ptr(arguments);
	return (tremppi_init(arg_ptr.getArgc(), arg_ptr.getArgv()));
}

int basic_update_test() {
	vector<string> arguments = { "--path", bfs::absolute(TremppiSystem::HOME_PATH / TEST_FOLDER / TEST_PROJECT).string() };
	ArgPtr arg_ptr(arguments);
	return (tremppi_update(arg_ptr.getArgc(), arg_ptr.getArgv()));
}