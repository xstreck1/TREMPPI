cmake_minimum_required (VERSION 3.0)
PROJECT(TREMPPI)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMake_SOURCE_DIR}/bin)

# 3rd party libaries are by default linked from the project repository
include_directories(${CMake_SOURCE_DIR})

# Boost
find_package(Boost 1.55.0)
include_directories(${Boost_INCLUDE_DIRS})

# Sqlite
ADD_LIBRARY(sqlite3 sqlite3/sqlite3.c)
ADD_LIBRARY(sqlite3pp sqlite3pp/sqlite3pp.cpp sqlite3pp/sqlite3ppext.cpp)

# Jsoncpp
ADD_LIBRARY(jsonccp  json/json_reader.cpp json/json_value.cpp json/json_writer.cpp)

#target_link_libraries(Minimizer ${Boost_LIBRARIES})
#FILE(GLOB MinimizerHeaders Minimizer/*.hpp)
#ADD_EXECUTABLE(Minimizer minimizer.cpp ${MinimizerHeaders})
