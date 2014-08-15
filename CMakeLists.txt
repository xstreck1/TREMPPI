cmake_minimum_required (VERSION 3.0)
PROJECT(TREMPPI)

# build to bin and lib
file(MAKE_DIRECTORY ${TREMPPI_SOURCE_DIR}/lib)
file(MAKE_DIRECTORY ${TREMPPI_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${TREMPPI_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${TREMPPI_SOURCE_DIR}/bin)

# configure a header file 
configure_file (
  "${PROJECT_SOURCE_DIR}/tremppi_common/configure.h.in"
  "${PROJECT_BINARY_DIR}/tremppi_common/configure.h"
)

# Boost
find_package(Boost 1.55.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
  
# Python
FIND_PACKAGE(PythonLibs REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHON_LIBRARY_DIRS})
  
# some 3rd party libraries are by linked from the project repository
include_directories(${TREMPPI_SOURCE_DIR})
# Sqlite
ADD_LIBRARY(sqlite3 sqlite3/sqlite3.c)
ADD_LIBRARY(sqlite3pp sqlite3pp/sqlite3pp.cpp sqlite3pp/sqlite3ppext.cpp)
# Jsoncpp
ADD_LIBRARY(jsonccp json/json_reader.cpp json/json_value.cpp json/json_writer.cpp)

# tremppi_common - code only, no binary
set(COMMON_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi_common)
FILE(GLOB_RECURSE COMMON_HEADERS ${COMMON_FOLDER}/*.hpp)
SOURCE_GROUP("Header Files\\tremppi_common\\database" ${COMMON_FOLDER}/database/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_common\\general" ${COMMON_FOLDER}/general/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_common\\network" ${COMMON_FOLDER}/network/.*hpp)
ADD_LIBRARY(tremppi_common tremppi_common/tremppi_common.cpp tremppi_common/header.h tremppi_common/configure.h ${COMMON_HEADERS})

# tremppi core
set(CORE_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi)
ADD_EXECUTABLE(tremppi ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} ${CORE_FOLDER}/tremppi.cpp)
target_link_libraries(tremppi ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})

# Gecode
add_subdirectory (gecode-4.2.1)
include_directories(${TREMPPI_SOURCE_DIR}/gecode-4.2.1)

# tremppi_spawn
set(SPAWN_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi_spawn)
FILE(GLOB_RECURSE SPAWN_HEADERS ${SPAWN_FOLDER}/*.hpp)
SOURCE_GROUP("Header Files\\tremppi_spawn\\compute" ${SPAWN_FOLDER}/compute/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_spawn\\data" ${SPAWN_FOLDER}/data/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_spawn\\io" ${SPAWN_FOLDER}/io/.*hpp)
ADD_EXECUTABLE(tremppi_spawn ${SPAWN_FOLDER}/tremppi_spawn.cpp ${SPAWN_HEADERS})
target_link_libraries(tremppi_spawn gecodekernel gecodeint gecodesearch gecodeminimodel jsonccp sqlite3 sqlite3pp ${Boost_LIBRARIES})

# tremppi_express
set(EXPRESS_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi_express)
FILE(GLOB_RECURSE EXPRESS_HEADERS ${EXPRESS_FOLDER}/*.hpp)
SOURCE_GROUP("Header Files\\tremppi_express\\compute" ${EXPRESS_FOLDER}/compute/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_express\\data" ${EXPRESS_FOLDER}/data/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_express\\io" ${EXPRESS_FOLDER}/io/.*hpp)
ADD_EXECUTABLE(tremppi_express ${EXPRESS_FOLDER}/tremppi_express.cpp ${EXPRESS_HEADERS})
target_link_libraries(tremppi_express sqlite3 sqlite3pp ${Boost_LIBRARIES})

# tremppi_report
set(REPORT_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi_report)
FILE(GLOB_RECURSE REPORT_HEADERS ${REPORT_FOLDER}/*.hpp)
SOURCE_GROUP("Header Files\\tremppi_report\\analysis" ${REPORT_FOLDER}/analysis/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_report\\data" ${REPORT_FOLDER}/data/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_report\\io" ${REPORT_FOLDER}/io/.*hpp)
ADD_EXECUTABLE(tremppi_report ${REPORT_FOLDER}/tremppi_report.cpp ${REPORT_HEADERS})
target_link_libraries(tremppi_report jsonccp sqlite3 sqlite3pp ${Boost_LIBRARIES})

# tremppi_test
find_package(GTEST)
if (GTEST_FOUND)
	include_directories(${GTEST_INCLUDE_DIR})
	set(TEST_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi_test)
	FILE(GLOB_RECURSE TEST_HEADERS ${TEST_FOLDER}/*.hpp)
	SOURCE_GROUP("Header Files\\common_tests" ${TEST_FOLDER}/common_tests/.*hpp)
	SOURCE_GROUP("Header Files\\report_tests" ${TEST_FOLDER}/report_tests/.*hpp)
	SOURCE_GROUP("Header Files\\spawn_tests" ${TEST_FOLDER}/spawn_tests/.*hpp)
	ADD_EXECUTABLE(tremppi_test ${TEST_FOLDER}/tremppi_test.cpp ${TEST_HEADERS})
	target_link_libraries(tremppi_test jsonccp sqlite3 sqlite3pp ${Boost_LIBRARIES} ${GTEST_LIBRARY_DEBUG} ${GTEST_MAIN_LIBRARY_DEBUG})
else (GTEST_FOUND)
	message ("GTest not found. The tests will not be available.")
endif (GTEST_FOUND)
