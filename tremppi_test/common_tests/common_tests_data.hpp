#pragma once

#include <gtest/gtest.h>

#include <tremppi_common/network/data_info.hpp>

class CommonTest : public ::testing::Test {
protected:
	RegInfos r_negative_loop;
	void SetUp() override {
		r_negative_loop.emplace_back(RegInfo
		{
			0,
			"neg_loop",
			1,
			{ { 0, string("K_neg_loop_0") }, { 1, string("K_neg_loop_1") } },
			{ { 0, { 1 } } },
			{ { 0, { { 0 } } }, { 1, { { 1 } } } }
		});
	}
};
