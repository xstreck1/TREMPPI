#include "python_tests/python_tests.hpp"
#include "common_tests/common_tests.hpp"
#include "spawn_tests/spawn_tests.hpp"
#include "express_tests/express_tests.hpp"
#include "validate_tests/validate_tests.hpp"
#include "report_tests/report_tests.hpp"

TEST(CoreTest, AllPrograms) {
	ASSERT_EQ(0, basic_init_test());
	ASSERT_EQ(0, basic_spawn_test());
	ASSERT_EQ(0, basic_express_test());
	ASSERT_EQ(0, basic_report_test());
	ASSERT_EQ(0, basic_validate_test());
}

int tremppi_test(int argc, char** argv) {
	tremppi_system.set("tremppi_test", argv[0], bfs::current_path());
    ::testing::InitGoogleTest( &argc, argv );
	int result = RUN_ALL_TESTS();

	return result;
}
