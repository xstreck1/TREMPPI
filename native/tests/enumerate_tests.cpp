/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#pragma once

#include "tests.hpp"

#include <tremppi_enumerate/compute/normalizer.hpp>

// Print the basic model
void createModel(const bfs::path & example_model_path) 
{
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
	ofstream data_file((example_model_path / DATA_FOLDER / bfs::path{ NETWORK_FILENAME }).string(), ios::out);
	string data = writer.write(root);
	data_file << data;
}
void createSelect(const bfs::path & example_model_path) 
{
	Json::Value all;
	all["recid"] = 0;
	all["name"] = "all";
	all["select"] = true;
	Json::Value root;
	root["records"].append(all);

	Json::StyledWriter writer;
	ofstream data_file((example_model_path / DATA_FOLDER / bfs::path{ SELECTION_FILENAME }).string(), ios::out);
	string data = writer.write(root);
	data_file << data;
}
int basic_enumerate_test() 
{
	vector<string> arguments = { "--path", bfs::absolute(TremppiSystem::HOME_PATH / TEST_FOLDER / TEST_PROJECT).string() };
	ArgPtr arg_ptr(arguments);
	createModel(arguments[1]);
	createSelect(arguments[1]);
	return (tremppi_enumerate(arg_ptr.getArgc(), arg_ptr.getArgv()));
}


TEST_F(CommonTest, Normalization)
{
	Normalizer normalizer(r_multi_val);
	EXPECT_FALSE(normalizer.isNormalized({ 0 , 1 ,2 , 1, 2, 0 }, r_multi_val));
	EXPECT_TRUE(normalizer.isNormalized({ 0 , 1 ,2 , 1, 2, 1 }, r_multi_val));
}