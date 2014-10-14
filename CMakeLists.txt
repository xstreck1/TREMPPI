cmake_minimum_required (VERSION 3.0)
PROJECT(TREMPPI)

SET(BUILD_STANDALONE OFF CACHE BOOL "Build also stand-alones")
SET(BUILD_TEST OFF CACHE BOOL "Build the test suite")

# build to bin and lib
file(MAKE_DIRECTORY ${TREMPPI_SOURCE_DIR}/lib)
file(MAKE_DIRECTORY ${TREMPPI_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${TREMPPI_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${TREMPPI_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${TREMPPI_SOURCE_DIR})

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
FIND_PACKAGE(PythonLibs 3.2 REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHON_LIBRARY_DIRS})
  
# some 3rd party libraries are by linked from the project repository
include_directories(${TREMPPI_SOURCE_DIR})
# Sqlite
ADD_LIBRARY(sqlite3 sqlite3/sqlite3.c)
ADD_LIBRARY(sqlite3pp sqlite3pp/sqlite3pp.cpp sqlite3pp/sqlite3ppext.cpp)
# Jsoncpp
ADD_LIBRARY(jsonccp json/json_reader.cpp json/json_value.cpp json/json_writer.cpp)

# Gecode
add_subdirectory (gecode-4.3.0)
include_directories(${TREMPPI_SOURCE_DIR}/gecode-4.3.0)

# tremppi_common - code only, no binary
set(COMMON_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi_common)
FILE(GLOB_RECURSE COMMON_FILES ${COMMON_FOLDER}/*.*pp )
SOURCE_GROUP("Header Files\\tremppi_common\\database" ${COMMON_FOLDER}/database/.*hpp)
SOURCE_GROUP("Source Files\\tremppi_common\\database" ${COMMON_FOLDER}/database/.*cpp)
SOURCE_GROUP("Header Files\\tremppi_common\\general" ${COMMON_FOLDER}/general/.*hpp)
SOURCE_GROUP("Source Files\\tremppi_common\\general" ${COMMON_FOLDER}/general/.*cpp)
SOURCE_GROUP("Header Files\\tremppi_common\\network" ${COMMON_FOLDER}/network/.*hpp)
SOURCE_GROUP("Source Files\\tremppi_common\\network" ${COMMON_FOLDER}/network/.*cpp)
ADD_LIBRARY(tremppi_common_lib tremppi_common/tremppi_common.cpp tremppi_common/header.h tremppi_common/configure.h ${COMMON_FILES})

# tremppi_python - code only, no binary
ADD_LIBRARY(tremppi_python_lib tremppi_python/tremppi_python.cpp)

# tremppi_spawn
set(SPAWN_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi_spawn)
FILE(GLOB_RECURSE SPAWN_HEADERS ${SPAWN_FOLDER}/*.*pp)
SOURCE_GROUP("Header Files\\tremppi_spawn\\compute" ${SPAWN_FOLDER}/compute/.*hpp)
SOURCE_GROUP("Source Files\\tremppi_spawn\\compute" ${SPAWN_FOLDER}/compute/.*cpp)
SOURCE_GROUP("Header Files\\tremppi_spawn\\data" ${SPAWN_FOLDER}/data/.*hpp)
SOURCE_GROUP("Source Files\\tremppi_spawn\\data" ${SPAWN_FOLDER}/data/.*cpp)
SOURCE_GROUP("Header Files\\tremppi_spawn\\io" ${SPAWN_FOLDER}/io/.*hpp)
SOURCE_GROUP("Source Files\\tremppi_spawn\\io" ${SPAWN_FOLDER}/io/.*cpp)
ADD_LIBRARY(tremppi_spawn_lib ${SPAWN_FOLDER}/tremppi_spawn.cpp ${SPAWN_HEADERS})
if(BUILD_STANDALONE)
	ADD_EXECUTABLE(tremppi_spawn ${SPAWN_FOLDER}/main.cpp ${SPAWN_FOLDER}/tremppi_spawn.cpp ${SPAWN_HEADERS})
	target_link_libraries(tremppi_spawn tremppi_common_lib gecodekernel gecodeint gecodesearch gecodeminimodel jsonccp sqlite3 sqlite3pp  ${Boost_LIBRARIES})
endif(BUILD_STANDALONE)

# tremppi_express
set(EXPRESS_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi_express)
FILE(GLOB_RECURSE EXPRESS_HEADERS ${EXPRESS_FOLDER}/*.*pp)
SOURCE_GROUP("Header Files\\tremppi_express\\compute" ${EXPRESS_FOLDER}/compute/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_express\\data" ${EXPRESS_FOLDER}/data/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_express\\io" ${EXPRESS_FOLDER}/io/.*hpp)
ADD_LIBRARY(tremppi_express_lib ${EXPRESS_FOLDER}/tremppi_express.cpp ${EXPRESS_HEADERS})
if(BUILD_STANDALONE)
	ADD_EXECUTABLE(tremppi_express ${EXPRESS_FOLDER}/main.cpp  ${EXPRESS_FOLDER}/tremppi_express.cpp ${EXPRESS_HEADERS})
	target_link_libraries(tremppi_express tremppi_common_lib sqlite3 sqlite3pp  ${Boost_LIBRARIES})
endif(BUILD_STANDALONE)

# tremppi_report
set(REPORT_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi_report)
FILE(GLOB_RECURSE REPORT_HEADERS ${REPORT_FOLDER}/*.*pp)
SOURCE_GROUP("Header Files\\tremppi_report\\analysis" ${REPORT_FOLDER}/analysis/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_report\\data" ${REPORT_FOLDER}/data/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_report\\io" ${REPORT_FOLDER}/io/.*hpp)
ADD_LIBRARY(tremppi_report_lib ${REPORT_FOLDER}/tremppi_report.cpp ${REPORT_HEADERS})
if(BUILD_STANDALONE)
	ADD_EXECUTABLE(tremppi_report ${REPORT_FOLDER}/main.cpp ${REPORT_FOLDER}/tremppi_report.cpp ${REPORT_HEADERS})
	target_link_libraries(tremppi_report tremppi_common_lib jsonccp sqlite3 sqlite3pp ${Boost_LIBRARIES})
endif(BUILD_STANDALONE)

# tremppi_validate
set(VALIDATE_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi_validate)
FILE(GLOB_RECURSE VALIDATE_HEADERS ${VALIDATE_FOLDER}/*.*pp)
SOURCE_GROUP("Header Files\\tremppi_validate\\data" ${VALIDATE_FOLDER}/data/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_validate\\io" ${VALIDATE_FOLDER}/io/.*hpp)
SOURCE_GROUP("Header Files\\tremppi_validate\\compute" ${VALIDATE_FOLDER}/compute/.*hpp)
SOURCE_GROUP("Source Files\\tremppi_validate\\data" ${VALIDATE_FOLDER}/data/.*cpp)
SOURCE_GROUP("Source Files\\tremppi_validate\\io" ${VALIDATE_FOLDER}/io/.*cpp)
SOURCE_GROUP("Source Files\\tremppi_validate\\compute" ${VALIDATE_FOLDER}/compute/.*cpp)

ADD_LIBRARY(tremppi_validate_lib ${VALIDATE_FOLDER}/tremppi_validate.cpp ${VALIDATE_HEADERS})
if(BUILD_STANDALONE)
	ADD_EXECUTABLE(tremppi_validate ${VALIDATE_FOLDER}/main.cpp ${VALIDATE_FOLDER}/tremppi_validate.cpp ${VALIDATE_HEADERS})
	target_link_libraries(tremppi_validate tremppi_common_lib jsonccp sqlite3 sqlite3pp ${Boost_LIBRARIES})
endif(BUILD_STANDALONE)

# tremppi_test
if (BUILD_TEST)
	find_package(GTEST)
	if (GTEST_FOUND)
		include_directories(${GTEST_INCLUDE_DIR})
		set(TEST_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi_test)
		FILE(GLOB_RECURSE TEST_HEADERS ${TEST_FOLDER}/*.*pp)
		SOURCE_GROUP("Header Files\\common_tests" ${TEST_FOLDER}/common_tests/.*hpp)
		SOURCE_GROUP("Header Files\\express_tests" ${TEST_FOLDER}/express_tests/.*hpp)
		SOURCE_GROUP("Header Files\\python_tests" ${TEST_FOLDER}/python_tests/.*hpp)
		SOURCE_GROUP("Header Files\\report_tests" ${TEST_FOLDER}/report_tests/.*hpp)
		SOURCE_GROUP("Header Files\\spawn_tests" ${TEST_FOLDER}/spawn_tests/.*hpp)
		SOURCE_GROUP("Header Files\\validate_tests" ${TEST_FOLDER}/validate_tests/.*hpp)
		ADD_EXECUTABLE(tremppi_test ${TEST_FOLDER}/tremppi_test.cpp ${TEST_FOLDER}/main.cpp ${TEST_HEADERS})
		target_link_libraries(tremppi_test debug  tremppi_common_lib tremppi_python_lib tremppi_validate_lib tremppi_spawn_lib tremppi_express_lib tremppi_report_lib gecodekernel gecodeint gecodesearch gecodeminimodel jsonccp sqlite3 sqlite3pp ${PYTHON_LIBRARIES}  ${Boost_LIBRARIES} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
	else (GTEST_FOUND)
		message ("GTest not found. The tests will not be available.")
	endif (GTEST_FOUND)
endif (BUILD_TEST)

# tremppi core
set(CORE_FOLDER ${TREMPPI_SOURCE_DIR}/tremppi)
ADD_EXECUTABLE(tremppi ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} ${CORE_FOLDER}/tremppi.cpp)
target_link_libraries(tremppi tremppi_common_lib tremppi_python_lib tremppi_validate_lib tremppi_spawn_lib tremppi_express_lib tremppi_report_lib gecodekernel gecodeint gecodesearch gecodeminimodel jsonccp sqlite3 sqlite3pp ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
