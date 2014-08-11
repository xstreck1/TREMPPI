cmake_minimum_required (VERSION 3.0)
PROJECT(TREMPPI)

# build to bin and lib
file(MAKE_DIRECTORY ${TREMPPI_SOURCE_DIR}/lib)
file(MAKE_DIRECTORY ${TREMPPI_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${TREMPPI_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${TREMPPI_SOURCE_DIR}/bin)

# configure a header file 
configure_file (
  "${PROJECT_SOURCE_DIR}/tpi_common/configure.h.in"
  "${PROJECT_BINARY_DIR}/tpi_common/configure.h"
)

# Boost
find_package(Boost 1.55.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
  
# some 3rd party libraries are by linked from the project repository
include_directories(${TREMPPI_SOURCE_DIR})
# Sqlite
ADD_LIBRARY(sqlite3 sqlite3/sqlite3.c)
ADD_LIBRARY(sqlite3pp sqlite3pp/sqlite3pp.cpp sqlite3pp/sqlite3ppext.cpp)
# Jsoncpp
ADD_LIBRARY(jsonccp json/json_reader.cpp json/json_value.cpp json/json_writer.cpp)

# tpi_common
set(COMMON_FOLDER ${TREMPPI_SOURCE_DIR}/tpi_common)
FILE(GLOB_RECURSE COMMON_HEADERS ${COMMON_FOLDER}/*.hpp)
ADD_LIBRARY(tpi_common tpi_common/tpi_common.cpp tpi_common/configure.h ${COMMON_HEADERS})

# Gecode
add_subdirectory (gecode-4.2.1)
include_directories(${TREMPPI_SOURCE_DIR}/gecode-4.2.1)

# tpi_spawn
set(SPAWN_FOLDER ${TREMPPI_SOURCE_DIR}/tpi_spawn)
FILE(GLOB_RECURSE SPAWN_HEADERS ${SPAWN_FOLDER}/*.hpp)
SOURCE_GROUP("Header Files\\io" ${SPAWN_FOLDER}/io/.*hpp)
ADD_EXECUTABLE(tpi_spawn ${SPAWN_FOLDER}/tpi_spawn.cpp ${SPAWN_HEADERS})
target_link_libraries(tpi_spawn gecodekernel gecodeint gecodesearch gecodeminimodel jsonccp sqlite3 sqlite3pp ${Boost_LIBRARIES})


# tpi_report
set(REPORT_FOLDER ${TREMPPI_SOURCE_DIR}/tpi_report)
FILE(GLOB_RECURSE REPORT_HEADERS ${REPORT_FOLDER}/*.hpp)
SOURCE_GROUP("Header Files\\io" ${REPORT_FOLDER}/io/.*hpp)
SOURCE_GROUP("Header Files\\data" ${REPORT_FOLDER}/data/.*hpp)
SOURCE_GROUP("Header Files\\analysis" ${REPORT_FOLDER}/analysis/.*hpp)
ADD_EXECUTABLE(tpi_report ${REPORT_FOLDER}/tpi_report.cpp ${REPORT_HEADERS})
target_link_libraries(tpi_report jsonccp sqlite3 sqlite3pp ${Boost_LIBRARIES})

# tpi_test
find_package(GTEST)
if (GTEST_FOUND)
	include_directories(${GTEST_INCLUDE_DIR})
	set(TEST_FOLDER ${TREMPPI_SOURCE_DIR}/tpi_test)
	FILE(GLOB_RECURSE TEST_HEADERS ${TEST_FOLDER}/*.hpp)
	SOURCE_GROUP("Header Files\\common_tests" ${TEST_FOLDER}/common_tests/.*hpp)
	SOURCE_GROUP("Header Files\\report_tests" ${TEST_FOLDER}/report_tests/.*hpp)
	SOURCE_GROUP("Header Files\\spawn_tests" ${TEST_FOLDER}/spawn_tests/.*hpp)
	ADD_EXECUTABLE(tpi_test ${TEST_FOLDER}/tpi_test.cpp ${TEST_HEADERS})
	target_link_libraries(tpi_test jsonccp sqlite3 sqlite3pp ${Boost_LIBRARIES} ${GTEST_LIBRARY_DEBUG} ${GTEST_MAIN_LIBRARY_DEBUG})
else (GTEST_FOUND)
	message ("GTest not found. The tests will not be available.")
endif (GTEST_FOUND)
