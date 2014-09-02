#pragma once

#include "../validate_tests/validate_tests_data.hpp"
#include <tremppi_validate/construction/construction_manager.hpp>


class ValidateTest : public CommonTest {
protected:
	PropertyAutomaton a_cycle_on_A;
	void SetUp() override {
		CommonTest::SetUp();

		a_cycle_on_A.name = "cycle_on_A";
		a_cycle_on_A.experiment = "";
		a_cycle_on_A.prop_type = "TimeSeries";
		a_cycle_on_A.states.emplace_back(PropertyAutomaton::State{ "0", 0, false });
		a_cycle_on_A.states[0].edges = vector < PropertyAutomaton::Edge > { PropertyAutomaton::Edge{ 0, "A=0" }, PropertyAutomaton::Edge{ 1, "A=1" }};
		a_cycle_on_A.states.emplace_back(PropertyAutomaton::State{ "1", 1, false });
		a_cycle_on_A.states[1].edges = vector < PropertyAutomaton::Edge > { PropertyAutomaton::Edge{ 1, "A=1" }, PropertyAutomaton::Edge{ 2, "A=0" }};
		a_cycle_on_A.states.emplace_back(PropertyAutomaton::State{ "2", 2, true });
	}
};
