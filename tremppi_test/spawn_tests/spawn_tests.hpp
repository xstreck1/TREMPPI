#pragma once

#include <gtest/gtest.h>

#include <tremppi_spawn/io/model_reader.hpp>
#include <tremppi_spawn/io/syntax_checker.hpp>

TEST(Spawn, ExampleModel) {
	bfs::path example_model_path(bfs::path{ TREMPPI_PATH } /= bfs::path{ "data/tremppi_test/basic_model.js" });
	EXPECT_TRUE(bfs::exists(example_model_path)) << "File " << example_model_path.string() << " does not exist.";
	Json::Value root;
	EXPECT_NO_THROW(root = ModelReader::readFile(example_model_path.string()));
	EXPECT_NO_THROW(SyntaxChecker::controlSemantics(root["elements"]));
}

