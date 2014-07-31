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

# 3rd party libaries are by default linked from the project repository
include_directories(${TREMPPI_SOURCE_DIR})

# Boost
find_package(Boost 1.55.0)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Sqlite
ADD_LIBRARY(sqlite3 sqlite3/sqlite3.c)
ADD_LIBRARY(sqlite3pp sqlite3pp/sqlite3pp.cpp sqlite3pp/sqlite3ppext.cpp)

# Jsoncpp
ADD_LIBRARY(jsonccp  json/json_reader.cpp json/json_value.cpp json/json_writer.cpp)

# tpi_common
#set(TPI_FOLDER ${TREMPPI_SOURCE_DIR}/tpi_common)
#FILE(GLOB_RECURSE TPI_HEADERS ${TPI_FOLDER}/*.hpp)
#ADD_LIBRARY(tpi_common ${TPI_HEADERS})

# tpi_stat
set(STAT_FOLDER ${TREMPPI_SOURCE_DIR}/tpi_stat)
FILE(GLOB_RECURSE STAT_HEADERS ${STAT_FOLDER}/*.hpp)
SOURCE_GROUP("Header Files\\io" ${STAT_FOLDER}/io/.*hpp)
SOURCE_GROUP("Header Files\\data" ${STAT_FOLDER}/data/.*hpp)
SOURCE_GROUP("Header Files\\analysis" ${STAT_FOLDER}/analysis/.*hpp)
ADD_EXECUTABLE(tpi_stat ${STAT_FOLDER}/tpi_stat.cpp ${STAT_HEADERS})
target_link_libraries(tpi_stat ${Boost_LIBRARIES} jsonccp sqlite3 sqlite3pp)
