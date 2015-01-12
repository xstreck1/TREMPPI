#pragma once

#include <tremppi_validate/compute/analysis_manager.hpp>
#include <tremppi_validate/io/validate_options.hpp>

#include "validate_tests_data.hpp"

int tremppi_validate(int, char**);

// Print the basic model
void createProperties(const bfs::path & example_model_path) {
	Json::Value root;
	root.resize(2);
	// Add time series
	root[0]["desc"].resize(1);
	root[0]["desc"][0]["id"] = 0;
	root[0]["desc"][0]["values"]["Name"] = "test_ts";
	root[0]["desc"][0]["values"]["Type"] = "TimeSeries";
	root[0]["desc"][0]["values"]["Experiment"] = "";
	root[0]["desc"][0]["values"]["Verify"] = true;
	root[0]["data"].resize(2);
	root[0]["data"][0]["id"] = 0;
	root[0]["data"][0]["values"]["Measurement"] = "B>0";
	root[0]["data"][1]["id"] = 1;
	root[0]["data"][1]["values"]["Measurement"] = "A>0&B>1";
	// Add a cycle
	root[1]["desc"].resize(1);
	root[1]["desc"][0]["id"] = 0;
	root[1]["desc"][0]["values"]["Name"] = "test_cycle";
	root[1]["desc"][0]["values"]["Type"] = "Cycle";
	root[1]["desc"][0]["values"]["Experiment"] = "";
	root[1]["desc"][0]["values"]["Verify"] = true;
	root[1]["data"].resize(2);
	root[1]["data"][0]["id"] = 0;
	root[1]["data"][0]["values"]["Measurement"] = "A=1";
	root[1]["data"][1]["id"] = 1;
	root[1]["data"][1]["values"]["Measurement"] = "A=0";


	Json::StyledWriter writer;
	ofstream data_file((example_model_path / PROPERTIES_FILENAME).string(), ios::out);
	string data = writer.write(root);
	data_file << data;
}

int basic_validate_test()  {
	const int argc = 5;
	char * argv[argc];

	bfs::path example_model_path(bfs::absolute(bfs::path{ tremppi_system.HOME_PATH } / "test" / "test_proj"));
	createProperties(example_model_path);
	argv[0] = new char[tremppi_system.BIN_PATH.string().size() + 1];
	strcpy(argv[0], tremppi_system.BIN_PATH.string().c_str());
	argv[1] = new char[7];
	strcpy(argv[1], "--path");
	argv[2] = new char[example_model_path.string().size() + 1];
	strcpy(argv[2], example_model_path.string().c_str());
	argv[3] = new char[8];
	strcpy(argv[3], "--trace"); 
	argv[4] = new char[4];
	strcpy(argv[4], "wit");

	int result = (tremppi_validate(argc, argv));

	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}

	return result;
}

TEST_F(ValidateTest, Construction) {
	// Construct unparametrized structure and check if all the values are as expected
	UnparametrizedStructureBuilder unparametrized_structure_builder(r_negative_loop, a_spike_on_A);
	UnparametrizedStructure unparametrized_structure = unparametrized_structure_builder.buildStructure();
	ASSERT_EQ(2, unparametrized_structure.getStateCount());
	EXPECT_EQ(Levels{ 0 }, unparametrized_structure.getStateLevels(0));
	EXPECT_EQ(Levels{ 1 }, unparametrized_structure.getStateLevels(1));
	ASSERT_EQ(1, unparametrized_structure.getTransitionCount(0));
	EXPECT_EQ(1, unparametrized_structure.getTargetID(0, 0));
	const TransConst & const_1 = unparametrized_structure.getTransitionConst(0, 0);
	EXPECT_EQ(0, const_1.param_no);
	EXPECT_EQ(true, const_1.req_dir);
	EXPECT_EQ(0, const_1.req_value);
	ASSERT_EQ(1, unparametrized_structure.getTransitionCount(1));
	EXPECT_EQ(0, unparametrized_structure.getTargetID(1, 0));
	const TransConst & const_2 = unparametrized_structure.getTransitionConst(1, 0);
	EXPECT_EQ(1, const_2.param_no);
	EXPECT_EQ(false, const_2.req_dir);
	EXPECT_EQ(1, const_2.req_value);

	// Create the Buchi automaton
	AutomatonBuilder automaton_builder(r_negative_loop, a_spike_on_A);
	AutomatonStructure automaton = automaton_builder.buildAutomaton();
	ASSERT_EQ(3, automaton.getStateCount());
	EXPECT_EQ(vector < StateID > {0}, automaton.getInitialStates());
	EXPECT_EQ(vector < StateID > {2}, automaton.getFinalStates());
	EXPECT_EQ(BA_finite, automaton.getMyType());
	ASSERT_EQ(2, automaton.getTransitionCount(0));
	EXPECT_EQ(0, automaton.getTargetID(0, 0));
	EXPECT_EQ(1, automaton.getTargetID(0, 1));
	ASSERT_EQ(2, automaton.getTransitionCount(1));
	EXPECT_EQ(1, automaton.getTargetID(1, 0));
	EXPECT_EQ(2, automaton.getTargetID(1, 1));
	EXPECT_EQ(0, automaton.getTransitionCount(2));

	// Create the product
	ProductBuilder product_builder;
	ProductStructure product = product_builder.buildProduct(move(unparametrized_structure), move(automaton));
	ASSERT_EQ(6, product.getStateCount());
	ASSERT_EQ(1, product.getTransitionCount(0));
	EXPECT_EQ(1, product.getTargetID(0, 0));
	ASSERT_EQ(1, product.getTransitionCount(1));
	EXPECT_EQ(2, product.getTargetID(1, 0));
	ASSERT_EQ(1, product.getTransitionCount(2));
	EXPECT_EQ(5, product.getTargetID(2, 0));
}

TEST_F(ValidateTest, SteadyStates) {
	ProductStructure p_unreagulated_is_steady = ConstructionManager::construct(r_unregulated, a_is_steady);
	AnalysisManager a_unreagulated_is_steady(p_unreagulated_is_steady);
	auto results = a_unreagulated_is_steady.checkFinite(INF, TraceType::wit, { 1 });
	EXPECT_EQ(2, get<0>(results)) << "Cost in SteadyStates should be 2--just one loop";
	EXPECT_EQ(1, get<1>(results).size()) << "Witness should contain exactly a single loop";
	EXPECT_EQ(1, get<1>(results).begin()->first) << "Witness should a loop (1,1)";
	EXPECT_DOUBLE_EQ(0.5, get<2>(results)) << "Robustnes should be half (no witness from 0)";
}

TEST_F(ValidateTest, BasicValidation) {
	ProductStructure p_two_circuit_spike_on_A = ConstructionManager::construct(r_two_circuit, a_spike_on_A);
	AnalysisManager a_two_circuit_spike_on_A(p_two_circuit_spike_on_A);
	auto results = a_two_circuit_spike_on_A.checkFinite(INF, TraceType::wit, { 0, 1, 1, 0 });
	EXPECT_EQ(3, get<0>(results)) << "Two-steps way for the spike prop.";
	EXPECT_EQ(2, get<1>(results).size()) << "Exactly two steps are present for the spike.";
	EXPECT_DOUBLE_EQ(1.0/4.0, get<2>(results)) << "Robustnes 1/4---deterministic path from one of the initial states.";
}

TEST_F(ValidateTest, CycleProperty) {
	ProductStructure p_unreagulated_is_steady = ConstructionManager::construct(r_negative_loop, a_cycle_on_A);
	AnalysisManager a_unreagulated_is_steady(p_unreagulated_is_steady);
	auto results = a_unreagulated_is_steady.checkFull(INF, TraceType::wit, { 1, 0 });
	EXPECT_EQ(4, get<0>(results)) << "One step plus two-step loop.";
	EXPECT_EQ(3, get<1>(results).size()) << "One step plus two-step loop.";
	EXPECT_DOUBLE_EQ(1.0 / 2.0, get<2>(results)) << "Robustnes 1/2---deterministic path with two initials.";
}