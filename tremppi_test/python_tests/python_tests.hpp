#pragma once

#include <gtest/gtest.h>

#include <boost/python/detail/wrap_python.hpp>
#include <tremppi_common/general/system.hpp>

int tremppi_init(int, char**);

int basic_init_test() {
	const int argc = 4;
	char * argv[argc];

	bfs::path test_path(bfs::absolute(bfs::path{ tremppi_system.HOME_PATH } /= bfs::path{ "test" }));
	argv[0] = new char[tremppi_system.BIN_PATH.string().size() + 1];
	strcpy(argv[0], tremppi_system.BIN_PATH.string().c_str());
	argv[1] = new char[7];
	strcpy(argv[1], "--path");
	argv[2] = new char[test_path.string().size() + 1];
	strcpy(argv[2], test_path.string().c_str());
	argv[3] = new char[10];
	strcpy(argv[3], "test_proj");

	int result = (tremppi_init(argc, argv));

	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}

	return result;
}

