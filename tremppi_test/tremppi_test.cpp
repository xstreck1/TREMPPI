#include "report_tests/report_tests.hpp"

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest( &argc, argv );
	int result = RUN_ALL_TESTS();

	return result;
}
