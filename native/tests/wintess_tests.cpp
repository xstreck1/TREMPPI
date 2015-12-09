#pragma once

#include "tests.hpp"

int basic_witness_test() 
{
	vector<string> arguments = { "--path", bfs::absolute(TremppiSystem::HOME_PATH / TEST_FOLDER / TEST_PROJECT).string() };
	ArgPtr arg_ptr(arguments);
	return (tremppi_witness(arg_ptr.getArgc(), arg_ptr.getArgv()));
}
