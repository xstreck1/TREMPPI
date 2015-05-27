#include <tremppi_common/general/program_options.hpp>

#include "python_tests.hpp"
#include "common_tests.hpp"
#include "spawn_tests.hpp"
#include "express_tests.hpp"
#include "validate_tests.hpp"
#include "interact_tests.hpp"
#include "correlate_tests.hpp"
#include "wintess_tests.hpp"
#include "quantitative_tests.hpp"
#include "qualitative_tests.hpp"

TEST(CoreTest, AllPrograms) {
	bfs::remove_all(TremppiSystem::HOME_PATH / TEST_FOLDER / TEST_PROJECT);
	ASSERT_EQ(0, basic_init_test()) << "Basic content test initiation failed.";
	ASSERT_EQ(0, basic_spawn_test()) << "Basic content test spawn failed.";
	ASSERT_EQ(0, basic_express_test()) << "Basic content test express failed.";
	ASSERT_EQ(0, basic_validate_test()) << "Basic content test validate failed.";
	ASSERT_EQ(0, basic_interact_test()) << "Basic content test interact failed.";
	ASSERT_EQ(0, basic_function_test()) << "Basic content test function failed.";
	ASSERT_EQ(0, basic_witness_test()) << "Basic content test witness failed.";
	ASSERT_EQ(0, basic_qualitative_test()) << "Basic content test qualitative failed.";
	ASSERT_EQ(0, basic_quantitative_test()) << "Basic content test quantitative failed.";
	ASSERT_EQ(0, basic_update_test()) << "Basic content test update failed.";
	cout << endl;
}

int tremppi_test(int argc, char** argv) {
	TremppiSystem::initiate<ProgramOptions>("tremppi_test", argc, argv);

	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	return result;
}