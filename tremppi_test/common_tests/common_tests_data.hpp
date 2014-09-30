#pragma once

#include <gtest/gtest.h>

#include <tremppi_common/network/data_info.hpp>

class CommonTest : public ::testing::Test {
protected:
	RegInfos r_negative_loop;
	RegInfos r_unregulated;

	void SetUp() override {
		r_negative_loop.emplace_back(RegInfo{0,	"A", 1});
		r_negative_loop[0].columns = map<size_t, string>{ { 0, string("K_neg_loop_0") }, { 1, string("K_neg_loop_1") } };
		r_negative_loop[0].regulators = map<size_t, Levels>{ { 0, { 1 } } };
		r_negative_loop[0].requirements = map<size_t, vector<Levels> >{ { 0, { { 0 } } }, { 1, { { 1 } } } };

		r_unregulated.emplace_back(RegInfo{ 0, "A", 1 });
		r_unregulated[0].columns = map<size_t, string>{ { 0, string("K_") } };
		r_unregulated[0].regulators = map<size_t, Levels>();
		r_unregulated[0].requirements = map<size_t, vector<Levels> >{ { 0, { } }};
	}
};
