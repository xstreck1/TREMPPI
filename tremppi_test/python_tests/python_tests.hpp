#pragma once

#include <gtest/gtest.h>

#include <boost/python/detail/wrap_python.hpp>
#include <tremppi_common/header.h>

int tremppi_init(int, char**);

int basic_init_test() {
	const int argc = 4;
	char * argv[argc];

	bfs::path test_path(bfs::absolute(bfs::path{ tremppi_system.HOME_PATH } /= bfs::path{ "proj" }));
	argv[0] = new char[tremppi_system.BIN_PATH.string().size() + 1];
	strcpy(argv[0], tremppi_system.BIN_PATH.string().c_str());
	argv[1] = new char[7];
	strcpy(argv[1], "--dest");
	argv[2] = new char[test_path.string().size() + 1];
	strcpy(argv[2], test_path.string().c_str());
	argv[3] = new char[5];
	strcpy(argv[3], "test");

	int result = (tremppi_init(argc, argv));

	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}

	return result;
}

