#pragma once

#include <gtest/gtest.h>
#include <tremppi_common/general/logging.hpp>

int tremppi_spawn(int, char**);

// Print the basic model
void createModel(const bfs::path & example_model_path) {
	Json::Value root;
	root["nodes"].resize(2);
	root["nodes"][0]["data"]["Name"] = "A";
	root["nodes"][0]["data"]["MaxActivity"] = 1;
	root["nodes"][0]["data"]["id"] = "n0";
	root["nodes"][1]["data"]["Name"] = "B";
	root["nodes"][1]["data"]["MaxActivity"] = 2;
	root["nodes"][1]["data"]["id"] = "n1";
	root["edges"].resize(2);
	root["edges"][0]["data"]["source"] = "n0";
	root["edges"][0]["data"]["target"] = "n1";
	root["edges"][0]["data"]["Threshold"] = 1;
	root["edges"][0]["data"]["Label"] = "+&!-";
	root["edges"][1]["data"]["source"] = "n1";
	root["edges"][1]["data"]["target"] = "n1";
	root["edges"][1]["data"]["Threshold"] = 2;
	root["edges"][1]["data"]["Label"] = "+";

	Json::StyledWriter writer;
	ofstream data_file((example_model_path / "editor" / bfs::path{ NETWORK_FILENAME }).string(), ios::out);
	string data = writer.write(root);
	data_file << "var elements = " << data << ";";
}

int basic_spawn_test() {
	const int argc = 3;
	char * argv[argc];

	bfs::path example_model_path(bfs::absolute(bfs::path{ tremppi_system.HOME_PATH } / "proj" / "test"));
	createModel(example_model_path);

	argv[0] = new char[tremppi_system.BIN_PATH.string().size() + 1];
	strcpy(argv[0], tremppi_system.BIN_PATH.string().c_str());
	argv[1] = new char[7];
	strcpy(argv[1], "--path");
	argv[2] = new char[example_model_path.string().size() + 1];
	strcpy(argv[2], example_model_path.string().c_str());

	int result = (tremppi_spawn(argc, argv));

	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}

	return result;
}

