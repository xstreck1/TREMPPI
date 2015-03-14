#pragma once

#include <gtest/gtest.h>

#include <boost/python/detail/wrap_python.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/python/python_functions.hpp>

#include "../common_tests/common_tests_data.hpp"

int tremppi_init(int, char**);
int tremppi_update(int, char**);

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

TEST(Python, Select) {
	PythonFunctions & python = PythonFunctions::getInstance();
	python.exec("from tremppi.select import selection_from_file");
	string selection;
	python.eval<string>("selection_from_file(" + PythonFunctions::reformPath(bfs::absolute(bfs::path{ TremppiSystem::HOME_PATH } / "javascript" / "select.json" )) + ")", selection);
}