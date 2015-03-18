#pragma once

#include <gtest/gtest.h>
#include <tremppi_common/general/system.hpp>

#include "../common_tests/common_tests_data.hpp"

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
	root["edges"][0]["data"]["Label"] = "Activating Only";
	root["edges"][1]["data"]["source"] = "n1";
	root["edges"][1]["data"]["target"] = "n1";
	root["edges"][1]["data"]["Threshold"] = 2;
	root["edges"][1]["data"]["Label"] = "Activating";

	Json::StyledWriter writer;
	ofstream data_file((example_model_path / bfs::path{ NETWORK_FILENAME }).string(), ios::out);
	string data = writer.write(root);
	data_file << data;
}

int basic_spawn_test() {
	const string path = bfs::absolute(bfs::path{ TremppiSystem::HOME_PATH } / "test" / "test_proj").string();
	vector<string> arguments = { "--path", path};
	ArgPtr arg_ptr(arguments);
	createModel(path);
	return (tremppi_spawn(arg_ptr.getArgc(), arg_ptr.getArgv()));
}

