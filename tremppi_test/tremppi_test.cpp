#include "python_tests/python_tests.hpp"
#include "common_tests/common_tests.hpp"
#include "spawn_tests/spawn_tests.hpp"
#include "express_tests/express_tests.hpp"
#include "validate_tests/validate_tests.hpp"
#include "report_tests/report_tests.hpp"

TEST(CoreTest, AllPrograms) {
	//ASSERT_EQ(0, basic_init_test()) << "Basic content test initiation failed.";
	//ASSERT_EQ(0, basic_spawn_test()) << "Basic content test spawn failed.";
	//ASSERT_EQ(0, basic_express_test()) << "Basic content test express failed.";
	//ASSERT_EQ(0, basic_report_test()) << "Basic content test report failed.";
	ASSERT_EQ(0, basic_validate_test()) << "Basic content test validate failed.";
	cout << endl;
}

int tremppi_test(int argc, char** argv) {
	try {
		tremppi_system.set("tremppi_test", argv[0], bfs::current_path().string());
	}
	catch (exception & e) {
		cerr << e.what() << endl;
		exit(1);
	}
    ::testing::InitGoogleTest( &argc, argv );
	int result = RUN_ALL_TESTS();

	return result;
}
