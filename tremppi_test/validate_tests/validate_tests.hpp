#pragma once

#include "validate_tests_data.hpp"

int tremppi_validate(int, char**);

int basic_validate_test()  {
	const int argc = 3;
	char * argv[argc];

	bfs::path example_model_path(bfs::absolute(bfs::path{ tremppi_system.HOME_PATH } / "proj" / "test"));
	argv[0] = new char[tremppi_system.BIN_PATH.string().size() + 1];
	strcpy(argv[0], tremppi_system.BIN_PATH.string().c_str());
	argv[1] = new char[7];
	strcpy(argv[1], "--path");
	argv[2] = new char[example_model_path.string().size() + 1];
	strcpy(argv[2], example_model_path.string().c_str());

	int result = (tremppi_validate(argc, argv));

	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}

	return result;
}

TEST_F(ValidateTest, Construction) {
	// Construct unparametrized structure and check if all the values are as expected
	UnparametrizedStructureBuilder unparametrized_structure_builder(r_negative_loop, a_cycle_on_A);
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
	AutomatonBuilder automaton_builder(r_negative_loop, a_cycle_on_A);
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

TEST_F(ValidateTest, Checking) {
	ProductStructure p_negative_loop_A_cycle_on_a = ConstructionManager::construct(r_negative_loop, a_cycle_on_A);
}