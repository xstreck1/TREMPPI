#pragma once

#include <gtest/gtest.h>

#include "common_tests_data.hpp"

int basic_bias_test() {
	vector<string> arguments = { "--path", bfs::absolute(TremppiSystem::HOME_PATH / TEST_FOLDER / TEST_PROJECT ).string() };
	ArgPtr arg_ptr(arguments);
	return tremppi_bias(arg_ptr.getArgc(), arg_ptr.getArgv());
}

int basic_correlations_test() {
	vector<string> arguments = { "--path", bfs::absolute(TremppiSystem::HOME_PATH / TEST_FOLDER / TEST_PROJECT).string() };
	ArgPtr arg_ptr(arguments);
	return tremppi_correlations(arg_ptr.getArgc(), arg_ptr.getArgv());

}
