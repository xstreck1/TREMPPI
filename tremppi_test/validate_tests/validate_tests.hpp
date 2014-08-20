#pragma once

#include <gtest/gtest.h>

#include <tremppi_common/header.h>

int tremppi_validate(int, char**);

TEST(Validate, ExampleModel) {
	const int argc = 2;
	char * argv[argc];

	bfs::path example_model_path(bfs::path{ tremppi_system.HOME_PATH } /= bfs::path{ "data/tremppi_test/basic_model.sqlite" });
	argv[0] = new char[tremppi_system.BIN_PATH.string().size() + 1];
	strcpy(argv[0], tremppi_system.BIN_PATH.string().c_str());
	argv[1] = new char[example_model_path.string().size() + 1];
	strcpy(argv[1], example_model_path.string().c_str());

	ASSERT_EQ(0, tremppi_validate(argc, argv));

	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}
}

