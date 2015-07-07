cmake_minimum_required (VERSION 3.0)
PROJECT(TREMPPI)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")
endif(UNIX)
if(TREMPPI_STATIC)
	MESSAGE("BUILDING STATIC")
	set(Boost_USE_STATIC_LIBS ON)
	if(UNIX)
		set(CMAKE_C_FLAGS "-DSQLITE_THREADSAFE=0 -DSQLITE_OMIT_LOAD_EXTENSION")
		set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
	endif(UNIX)
endif(TREMPPI_STATIC)

set(CMAKE_INCLUDE_CURRENT_DIR ON)



set(NATIVE_SOURCE_DIR ${TREMPPI_SOURCE_DIR}/native)

# build to bin and lib
file(MAKE_DIRECTORY ${TREMPPI_SOURCE_DIR}/lib)
file(MAKE_DIRECTORY ${TREMPPI_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${TREMPPI_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${TREMPPI_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${TREMPPI_SOURCE_DIR}/cmake)

# Boost
find_package(Boost 1.54.0 COMPONENTS system program_options filesystem python REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
MESSAGE(${Boost_LIBRARIES})

SET(BUILD_STANDALONE OFF CACHE BOOL "Build also stand-alones")
SET(BUILD_TEST OFF CACHE BOOL "Build the test suite")

# Gecode
if(GECODE_LOCAL) # GECODE is inserted into the project
	add_subdirectory (${GECODE_LOCAL})
	include_directories(${TREMPPI_SOURCE_DIR}/${GECODE_LOCAL})
	link_directories(${TREMPPI_SOURCE_DIR}/${GECODE_LOCAL})
	set(Gecode_LIBRARIES gecodekernel gecodeint gecodesearch gecodeminimodel)
	set(Gecode_LIBRARIES_D)
else(GECODE_LOCAL) # GECODE is to be found outside
	FIND_PACKAGE(Gecode)
	include_directories(${Gecode_INCLUDE_DIRS})
endif(GECODE_LOCAL)

# configure a header file 
configure_file (
  "${NATIVE_SOURCE_DIR}/tremppi_common/configure.h.in"
  "${NATIVE_SOURCE_DIR}/tremppi_common/configure.h"
)
  
if (UNIX)
	if(TREMPPI_STATIC)
		set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
		set(CMAKE_EXE_LINKER_FLAGS "-static")
		set(Boost_LIBRARIES ${Boost_LIBRARIES} pthread dl util)
	endif(TREMPPI_STATIC)
endif(UNIX)

# Python
FIND_PACKAGE(PythonLibs 3.2 REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHON_LIBRARY_DIRS})
  
# some 3rd party libraries are by linked from the project repository
include_directories(${NATIVE_SOURCE_DIR})
# Sqlite
ADD_LIBRARY(sqlite3 ${NATIVE_SOURCE_DIR}/sqlite3/sqlite3.c)
ADD_LIBRARY(sqlite3pp ${NATIVE_SOURCE_DIR}/sqlite3pp/sqlite3pp.cpp ${NATIVE_SOURCE_DIR}/sqlite3pp/sqlite3ppext.cpp)
target_link_libraries(sqlite3pp sqlite3 ${Boost_LIBRARIES})
# Jsoncpp
ADD_LIBRARY(jsonccp ${NATIVE_SOURCE_DIR}/json/json_reader.cpp ${NATIVE_SOURCE_DIR}/json/json_value.cpp ${NATIVE_SOURCE_DIR}/json/json_writer.cpp)

# tremppi_common - code only, no binary
set(COMMON_FOLDER ${NATIVE_SOURCE_DIR}/tremppi_common)
FILE(GLOB_RECURSE COMMON_FILES ${COMMON_FOLDER}/*.*pp )
SOURCE_GROUP("Header Files\\database" ${COMMON_FOLDER}/database/.*hpp)
SOURCE_GROUP("Source Files\\database" ${COMMON_FOLDER}/database/.*cpp)
SOURCE_GROUP("Header Files\\general" ${COMMON_FOLDER}/general/.*hpp)
SOURCE_GROUP("Source Files\\general" ${COMMON_FOLDER}/general/.*cpp)
SOURCE_GROUP("Header Files\\network" ${COMMON_FOLDER}/network/.*hpp)
SOURCE_GROUP("Source Files\\network" ${COMMON_FOLDER}/network/.*cpp)
SOURCE_GROUP("Header Files\\report" ${COMMON_FOLDER}/report/.*hpp)
SOURCE_GROUP("Source Files\\report" ${COMMON_FOLDER}/report/.*cpp)
SOURCE_GROUP("Header Files\\python" ${COMMON_FOLDER}/python/.*hpp)
SOURCE_GROUP("Source Files\\python" ${COMMON_FOLDER}/python/.*cpp)
ADD_LIBRARY(tremppi_common ${NATIVE_SOURCE_DIR}/tremppi_common/header.h ${NATIVE_SOURCE_DIR}/tremppi_common/configure.h ${COMMON_FILES})

target_link_libraries(tremppi_common ${Boost_LIBRARIES} ${Gecode_LIBRARIES} ${Gecode_LIBRARIES_D} ${PYTHON_LIBRARIES} sqlite3 sqlite3pp jsonccp)


# tremppi_test
if (BUILD_TEST)
	find_package(GTEST)
	if (GTEST_FOUND)
		include_directories(${GTEST_INCLUDE_DIR})
		set(TEST_FOLDER ${NATIVE_SOURCE_DIR}/tests)
		FILE(GLOB TEST_HEADERS ${TEST_FOLDER}/*.hpp)
		ADD_EXECUTABLE(tests ${TEST_FOLDER}/tests.cpp ${TEST_FOLDER}/main.cpp ${TEST_HEADERS})
		target_link_libraries(tests tremppi_common  ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
	else (GTEST_FOUND)
		message ("GTest not found. The tests will not be available.")
	endif (GTEST_FOUND)
endif (BUILD_TEST)

# tremppi core
set(CORE_FOLDER ${NATIVE_SOURCE_DIR}/tremppi)
ADD_EXECUTABLE(tremppi ${CORE_FOLDER}/tremppi.cpp)
target_link_libraries(tremppi tremppi_common)

#tremppi module function
function(module name)
	set(FOLDER ${NATIVE_SOURCE_DIR}/tremppi_${name})
	FILE(GLOB_RECURSE HEADERS ${FOLDER}/*.*pp)
	SOURCE_GROUP("Headers ${name}\\compute" ${FOLDER}/compute/.*hpp)
	SOURCE_GROUP("Sources ${name}\\compute" ${FOLDER}/compute/.*cpp)
	SOURCE_GROUP("Headers ${name}\\data" ${FOLDER}/data/.*hpp)
	SOURCE_GROUP("Sources ${name}\\data" ${FOLDER}/data/.*cpp)
	SOURCE_GROUP("Headers ${name}\\io" ${FOLDER}/io/.*hpp)
	SOURCE_GROUP("Sources ${name}\\io" ${FOLDER}/io/.*cpp)
	SOURCE_GROUP("Sources ${name}" ${FOLDER}/.*cpp)
	ADD_LIBRARY(tremppi_${name} ${FOLDER}/tremppi_${name}.cpp ${HEADERS})
	target_link_libraries(tremppi_${name} tremppi_common)
	if(BUILD_STANDALONE)
		ADD_EXECUTABLE(${name} ${FOLDER}/main.cpp ${FOLDER}/tremppi_spawn.cpp ${HEADERS})
		target_link_libraries(${name} tremppi_common)
	endif(BUILD_STANDALONE)
	if (GTEST_FOUND)
		target_link_libraries(tests tremppi_${name})
	endif(GTEST_FOUND)
	target_link_libraries(tremppi tremppi_${name})
endfunction()

# MODULES
module("bias")
module("express")
module("sign")
module("regulations")
module("quantitative")
module("qualitative")
module("spawn")
module("validate")
module("witness")

