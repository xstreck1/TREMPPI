#include "python_tests/python_tests.hpp"
#include "common_tests/common_tests.hpp"
#include "spawn_tests/spawn_tests.hpp"
#include "express_tests/express_tests.hpp"
#include "validate_tests/validate_tests.hpp"
#include "report_tests/report_tests.hpp"

int tremppi_test(int argc, char** argv) {
    ::testing::InitGoogleTest( &argc, argv );
	int result = RUN_ALL_TESTS();

	return result;
}
