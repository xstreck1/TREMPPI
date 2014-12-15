#pragma once

#include "../common_tests/common_tests_data.hpp"
#include <tremppi_validate/data/construction_manager.hpp>

class ValidateTest : public CommonTest {
protected:
	PropertyAutomaton a_spike_on_A;
	PropertyAutomaton a_is_steady;
	PropertyAutomaton a_cycle_on_A;

	void SetUp() override {
		CommonTest::SetUp();

		a_spike_on_A.name = "spike_on_A";
		a_spike_on_A.experiment = "";
		a_spike_on_A.prop_type = "TimeSeries";
		a_spike_on_A.states.emplace_back(PropertyAutomaton::State{ "0", 0, false });
		a_spike_on_A.states[0].edges = vector < PropertyAutomaton::Edge > { PropertyAutomaton::Edge{ 0, "A=0" }, PropertyAutomaton::Edge{ 1, "A=1" }};
		a_spike_on_A.states.emplace_back(PropertyAutomaton::State{ "1", 1, false });
		a_spike_on_A.states[1].edges = vector < PropertyAutomaton::Edge > { PropertyAutomaton::Edge{ 1, "A=1" }, PropertyAutomaton::Edge{ 2, "A=0" }};
		a_spike_on_A.states.emplace_back(PropertyAutomaton::State{ "2", 2, true });

		a_is_steady.name = "is_steady";
		a_is_steady.prop_type = "TimeSeries";
		a_is_steady.states.emplace_back(PropertyAutomaton::State{ "0", 0, false });
		a_is_steady.states[0].edges = vector < PropertyAutomaton::Edge > { PropertyAutomaton::Edge{ 0, "!#ss" }, PropertyAutomaton::Edge{ 1, "#ss" }};
		a_is_steady.states.emplace_back(PropertyAutomaton::State{ "1", 1, true });


		a_cycle_on_A.name = "cycle_on_A";
		a_cycle_on_A.experiment = "";
		a_cycle_on_A.prop_type = "Cycle";
		a_cycle_on_A.states.emplace_back(PropertyAutomaton::State{ "0", 0, false });
		a_cycle_on_A.states[0].edges = vector < PropertyAutomaton::Edge > { PropertyAutomaton::Edge{ 0, "A=0" }, PropertyAutomaton::Edge{ 1, "A=1" }};
		a_cycle_on_A.states.emplace_back(PropertyAutomaton::State{ "1", 1, true });
		a_cycle_on_A.states[1].edges = vector < PropertyAutomaton::Edge > { PropertyAutomaton::Edge{ 2, "A=1" }, PropertyAutomaton::Edge{ 0, "A=0" }};
		a_cycle_on_A.states.emplace_back(PropertyAutomaton::State{ "2", 2, false });
		a_cycle_on_A.states[2].edges = vector < PropertyAutomaton::Edge > { PropertyAutomaton::Edge{ 2, "A=1" }, PropertyAutomaton::Edge{ 1, "A=0" }};
	}
};
