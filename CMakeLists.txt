cmake_minimum_required (VERSION 3.0)
PROJECT(TREMPPI)

if (TREMPPI_STATIC)
	SET(Boost_USE_STATIC_LIBS ON)
	if(UNIX)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
	endif(UNIX)
endif (TREMPPI_STATIC)
if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif(UNIX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(NATIVE_SOURCE_DIR ${TREMPPI_SOURCE_DIR}/native)

# build to bin and lib
file(MAKE_DIRECTORY ${TREMPPI_SOURCE_DIR}/lib)
set(LIBRARY_OUTPUT_PATH ${TREMPPI_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH ${TREMPPI_SOURCE_DIR}/cmake)

# Boost
add_definitions( -DBOOST_ALL_NO_LIB ) #disable autolink
find_package(Boost  1.54.0 COMPONENTS python-py35)
set(BOOST_PYTHON_LIB_NAME python-py35)
if (NOT Boost_FOUND) 
	find_package(Boost 1.54.0 COMPONENTS python-py34)
	set(BOOST_PYTHON_LIB_NAME python-py34)
endif(NOT Boost_FOUND)
if (NOT Boost_FOUND) 
	find_package(Boost 1.54.0 COMPONENTS python-py33)
	set(BOOST_PYTHON_LIB_NAME python-py33)
endif(NOT Boost_FOUND)
if (NOT Boost_FOUND) 
	find_package(Boost 1.54.0 COMPONENTS python-py32)
	set(BOOST_PYTHON_LIB_NAME python-py32)
endif(NOT Boost_FOUND)
if (NOT Boost_FOUND)  
	find_package(Boost 1.54.0 COMPONENTS python3)
	set(BOOST_PYTHON_LIB_NAME python3)
endif(NOT Boost_FOUND)
if (NOT Boost_FOUND)  
	find_package(Boost 1.54.0 COMPONENTS python REQUIRED)
	set(BOOST_PYTHON_LIB_NAME python)
	message("Found no Boost Python3 library. Linking to Boost Python, however this will cause version mis-match, if it was compiled for Python2.")
endif(NOT Boost_FOUND)
find_package(Boost 1.54.0 COMPONENTS ${BOOST_PYTHON_LIB_NAME} system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

MESSAGE("BOOST LIB" ${Boost_LIBRARY_DIRS})

# Python
set(Python_ADDITIONAL_VERSIONS 3.2 3.3 3.4 3.5)
FIND_PACKAGE(PythonLibs 3.2 REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHON_LIBRARY_DIRS})

SET(BUILD_TEST OFF CACHE BOOL "Build the test suite")

# Gecode
if(GECODE_LOCAL) # GECODE is inserted into the project
	add_subdirectory (${GECODE_LOCAL})
	include_directories(${TREMPPI_SOURCE_DIR}/${GECODE_LOCAL})
	link_directories(${TREMPPI_SOURCE_DIR}/${GECODE_LOCAL})
	set(Gecode_LIBRARIES gecodekernel gecodeint gecodesearch gecodeminimodel)
	set(Gecode_LIBRARIES_D gecodekernel gecodeint gecodesearch gecodeminimodel)
else(GECODE_LOCAL) # GECODE is to be found outside
	FIND_PACKAGE(Gecode)
	include_directories(${Gecode_INCLUDE_DIRS})
endif(GECODE_LOCAL)

# configure a header file 
configure_file (
  "${NATIVE_SOURCE_DIR}/tremppi_common/configure.h.in"
  "${NATIVE_SOURCE_DIR}/tremppi_common/configure.h"
)
  
# some 3rd party libraries are by linked from the project repository
include_directories(${NATIVE_SOURCE_DIR})
# Sqlite
ADD_LIBRARY(sqlite3 ${NATIVE_SOURCE_DIR}/sqlite3/sqlite3.c)
ADD_LIBRARY(sqlite3pp ${NATIVE_SOURCE_DIR}/sqlite3pp/sqlite3pp.cpp ${NATIVE_SOURCE_DIR}/sqlite3pp/sqlite3ppext.cpp)
if(UNIX)
	target_link_libraries(sqlite3 dl pthread)
endif(UNIX)
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
if (UNIX)
	target_link_libraries(tremppi_common dl util)
endif(UNIX)


# tremppi_test
if (BUILD_TEST)
	find_package(GTEST)
	if (GTEST_FOUND)
		include_directories(${GTEST_INCLUDE_DIR})
		set(TEST_FOLDER ${NATIVE_SOURCE_DIR}/tests)
		FILE(GLOB TEST_HEADERS ${TEST_FOLDER}/*.hpp)
		FILE(GLOB TEST_SOURCE ${TEST_FOLDER}/*.cpp)
		ADD_EXECUTABLE(tests ${TEST_FOLDER}/tests.cpp ${TEST_FOLDER}/main.cpp ${TEST_HEADERS} ${TEST_SOURCE})
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
	if (GTEST_FOUND)
		target_link_libraries(tests tremppi_${name})
	endif(GTEST_FOUND)
	target_link_libraries(tremppi tremppi_${name})
endfunction()

# MODULES
module("bias")
module("correlations")
module("express")
module("impact")
module("sign")
module("regulations")
module("quantitative")
module("qualitative")
module("spawn")
module("validate")
module("witness")