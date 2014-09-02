#pragma once

#include "validate_tests_data.hpp"

int tremppi_validate(int, char**);

int basic_validate_test()  {
	const int argc = 3;
	char * argv[argc];

	bfs::path example_model_path(bfs::absolute(bfs::path{ tremppi_system.HOME_PATH } / "proj" / "test"));
	argv[0] = new char[tremppi_system.BIN_PATH.string().size() + 1];
	strcpy(argv[0], tremppi_system.BIN_PATH.string().c_str());
	argv[1] = new char[7];
	strcpy(argv[1], "--path");
	argv[2] = new char[example_model_path.string().size() + 1];
	strcpy(argv[2], example_model_path.string().c_str());

	int result = (tremppi_validate(argc, argv));

	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}

	return result;
}

TEST_F(ValidateTest, Construction) {
	ProductStructure p_negative_loop_A_cycle_on_a = ConstructionManager::construct(r_negative_loop, a_cycle_on_A);
}