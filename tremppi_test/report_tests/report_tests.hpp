#pragma once

#include <gtest/gtest.h>
#include <tremppi_common/header.h>

int tremppi_report(int, char**);

int basic_report_test() {
	int argc = 4;
	char * argv[4];

	bfs::path example_model_path(bfs::absolute(bfs::path{ tremppi_system.HOME_PATH } / "test" / "test_proj"));
	argv[0] = new char[tremppi_system.BIN_PATH.string().size() + 1];
	strcpy(argv[0], tremppi_system.BIN_PATH.string().c_str());
	argv[1] = new char[7];
	strcpy(argv[1], "--path");
	argv[2] = new char[example_model_path.string().size() + 1];
	strcpy(argv[2], example_model_path.string().c_str());
	argv[3] = new char [3];
	strcpy(argv[3], "-r");

	int result = (tremppi_report(argc, argv));

	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}

	return result;
}
