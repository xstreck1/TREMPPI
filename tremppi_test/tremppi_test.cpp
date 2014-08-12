#include "common_tests/common_tests.hpp"
#include "report_tests/report_tests.hpp"
#include "spawn_tests/spawn_tests.hpp"

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest( &argc, argv );
	int result = RUN_ALL_TESTS();

	return result;
}
