#pragma once

#include <gtest/gtest.h>
#include <tremppi_common/general/logging.hpp>

int tremppi_spawn(int, char**);

int basic_spawn_test() {
	const int argc = 2;
	char * argv[argc];

	bfs::path example_model_path(bfs::path{ tremppi_system.HOME_PATH } /= bfs::path{ "proj/test/test.js" });
	argv[0] = new char[tremppi_system.BIN_PATH.string().size() + 1];
	strcpy(argv[0], tremppi_system.BIN_PATH.string().c_str());
	argv[1] = new char[example_model_path.string().size() + 1];
	strcpy(argv[1], example_model_path.string().c_str());

	int result = (tremppi_spawn(argc, argv));

	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}

	return result;
}

