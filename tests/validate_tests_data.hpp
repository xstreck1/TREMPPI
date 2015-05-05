#pragma once

#include <tremppi_validate/data/construction_manager.hpp>

#include "common_tests_data.hpp"

class ValidateTest : public CommonTest {
protected:
	PropertyInfo a_spike_on_A;
	PropertyInfo a_is_steady;
	PropertyInfo a_cycle_on_A;

	void SetUp() override {
		CommonTest::SetUp();

		a_spike_on_A.name = "spike_on_A";
		a_spike_on_A.ending = "any";
		a_spike_on_A.measurements.emplace_back(PropertyInfo::Measurement{ 0, { {"A", {1,2} } }, {} });
		a_spike_on_A.bound = INF;

		a_is_steady.name = "is_steady";
		a_is_steady.ending = "stable";
		a_spike_on_A.measurements.emplace_back(PropertyInfo::Measurement{ 0, {}, {} });
		a_is_steady.bound = INF;

		a_cycle_on_A.name = "cycle_on_A";
		a_cycle_on_A.ending = "goto A";
		a_spike_on_A.measurements.emplace_back(PropertyInfo::Measurement{ 0,{ { "A",{ 1,2 } },{ "A",{ 0,1 } } },{} });
		a_cycle_on_A.bound = INF;
	}
};
