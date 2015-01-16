#pragma once

#include <tremppi_validate/compute/analysis_manager.hpp>
#include <tremppi_validate/io/validate_options.hpp>

#include "witness_tests_data.hpp"

int tremppi_witness(int, char**);

int basic_witness_test() {
	vector<string> arguments = { "--path", bfs::absolute(bfs::path{ TremppiSystem::HOME_PATH } / "test" / "test_proj").string() };
	ArgPtr arg_ptr(arguments);
	return (tremppi_witness(arg_ptr.getArgc(), arg_ptr.getArgv()));
}
