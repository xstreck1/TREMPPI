#pragma once

#include <tremppi_validate/compute/analysis_manager.hpp>

#include "validate_tests_data.hpp"

int tremppi_validate(int, char**);

// Print the basic model
void createProperties(const bfs::path & example_model_path) {
	Json::Value root;
	Json::Value & records = root["list"]["records"];
	records.resize(2);

	// Add time series
	records[0]["name"] = "test_ts";
	records[0]["ending"] = "any";
	records[0]["E_A"] = "";
	records[0]["E_B"] = "";
	records[0]["validate"] = true;
	records[0]["robustness"] = true;
	records[0]["witness"] = false;
	records[0]["records"].resize(2);
	records[0]["records"][0]["V_B"] = "[0,1)";
	records[0]["records"][1]["V_B"] = "(1,2]";

	// Add a cycle
	records[1]["name"] = "test_cycle";
	records[1]["ending"] = "goto A";
	records[1]["E_A"] = "";
	records[1]["E_B"] = "";
	records[1]["validate"] = true;
	records[1]["robustness"] = false;
	records[1]["witness"] = true;
	records[1]["records"].resize(2);
	records[1]["records"][0]["V_B"] = "[0,1)";
	records[1]["records"][1]["V_B"] = "[1,1]";

	Json::StyledWriter writer;
	ofstream data_file((example_model_path / DATA_FOLDER / PROPERTIES_FILENAME).string(), ios::out);
	string data = writer.write(root);
	data_file << data;
}

int basic_validate_test() {
	vector<string> arguments = { "--path", bfs::absolute(TremppiSystem::HOME_PATH / TEST_FOLDER / TEST_PROJECT).string() };
	ArgPtr arg_ptr(arguments);
	createProperties(arguments[1]);
	int res = tremppi_validate(arg_ptr.getArgc(), arg_ptr.getArgv());
	/* fstream fout(bfs::absolute(bfs::path{ TremppiSystem::HOME_PATH } / "test" / "test_proj" / "select.json").string(), ios::out);
	fout << "[{\"id\": 0, \"values\" : {\"Selection\": \"C_test_ts\", \"Select\" : true, \"Compare\" : false}}]";*/
	return res;
}

TEST_F(ValidateTest, Construction) {
	auto bounds = ConstructionManager::getBounds(r_negative_loop, a_spike_on_A);
	ASSERT_EQ(1, get<0>(bounds).size());
	EXPECT_EQ(0, get<0>(bounds)[0]) << "Min is 0";
	ASSERT_EQ(1, get<1>(bounds).size());
	EXPECT_EQ(1, get<1>(bounds)[0]) << "Max is 1";
	ASSERT_EQ(1, get<2>(bounds).size());
	EXPECT_EQ(2, get<2>(bounds)[0]) << "Two values in total";

	// Construct unparametrized structure and check if all the values are as expected
	UnparametrizedStructure us;
	us._bounds = bounds;
	UnparametrizedStructureBuilder::buildStructure(r_negative_loop, us);
	ASSERT_EQ(2, us.size());
	EXPECT_EQ(Levels{ 0 }, us._states[0]._levels);
	EXPECT_EQ(Levels{ 1 }, us._states[1]._levels);
	ASSERT_EQ(1, us._states[0].size());
	EXPECT_EQ(1, us._states[0]._transitions[0]._t_ID);
	const TransConst & const_1 = us._states[0]._transitions[0]._trans_const;
	EXPECT_EQ(0, const_1._param_no);
	EXPECT_EQ(true, const_1._req_dir);
	EXPECT_EQ(0, const_1._req_value);
	ASSERT_EQ(1, us._states[1].size());
	EXPECT_EQ(0, us._states[1]._transitions[0]._t_ID);
	const TransConst & const_2 = us._states[1]._transitions[0]._trans_const;
	EXPECT_EQ(1, const_2._param_no);
	EXPECT_EQ(false, const_2._req_dir);
	EXPECT_EQ(1, const_2._req_value);

	// Create the Buchi automaton
	AutomatonStructure a;
	AutomatonBuilder::buildAutomaton(a_spike_on_A, bounds, { "A" }, a);
	ASSERT_EQ(2, a.size());
	EXPECT_EQ(vector < StateID > {0}, a.getInitialStates());
	EXPECT_EQ(vector < StateID > {1}, a.getFinalStates());
	EXPECT_EQ(BA_finite, a._aut_type);
	ASSERT_EQ(2, a._states[0].size());
	EXPECT_EQ(0, a._states[0]._transitions[0]._t_ID);

	// Create the product
	ProductStructure p;
	ProductBuilder::buildProduct(us, a, p);
	ASSERT_EQ(4, p.size());
	ASSERT_EQ(1, p._states[0].size());
	EXPECT_EQ(1, p._states[0]._transitions[0]._t_ID);
	ASSERT_EQ(1, p._states[0].size());
	EXPECT_EQ(2, p._states[1]._transitions[0]._t_ID);
}

TEST_F(ValidateTest, SteadyStates) {
	ProductStructure p_unreagulated_is_steady;
	ConstructionManager::construct(r_unregulated, a_is_steady, p_unreagulated_is_steady);
	AnalysisManager a_unreagulated_is_steady(p_unreagulated_is_steady, INF, true, true);
	auto results = a_unreagulated_is_steady.check({ 1 });
	EXPECT_EQ(1, get<0>(results)) << "Cost in SteadyStates should be 1--just one state";
	ASSERT_EQ(0, get<1>(results).size()) << "Witness is empty---just a single state.";
	EXPECT_DOUBLE_EQ(1, get<2>(results)) << "Robustnes should be 1";
}

TEST_F(ValidateTest, BasicValidation) {
	ProductStructure p_two_circuit_spike_on_A;
	ConstructionManager::construct(r_two_circuit, a_spike_on_A, p_two_circuit_spike_on_A);
	AnalysisManager a_two_circuit_spike_on_A(p_two_circuit_spike_on_A, INF, true, true);
	auto results = a_two_circuit_spike_on_A.check({ 0, 1, 1, 0 });
	// std::cout << WitnessSearcher::getOutput(p_two_circuit_spike_on_A, get<0>(results), get<1>(results));
	EXPECT_EQ(4, get<0>(results)) << "Two-steps way for the spike prop.";
	ASSERT_EQ(3, get<1>(results).size()) << "Exactly two steps are present for the spike.";
	EXPECT_DOUBLE_EQ(0.5, get<2>(results)) << "Robustnes 1---deterministic path from one of two initials.";
}

TEST_F(ValidateTest, CycleProperty) {
	ProductStructure p_unreagulated_is_steady;
	ConstructionManager::construct(r_negative_loop, a_cycle_on_A, p_unreagulated_is_steady);
	AnalysisManager a_unreagulated_is_steady(p_unreagulated_is_steady, INF, true, true);
	auto results = a_unreagulated_is_steady.check({ 1, 0 });
	EXPECT_EQ(3, get<0>(results)) << "Two-step loop.";
	ASSERT_EQ(2, get<1>(results).size()) << "Two-step loop.";
	EXPECT_DOUBLE_EQ(1.0 , get<2>(results)) << "Robustnes 1---deterministic path.";
}