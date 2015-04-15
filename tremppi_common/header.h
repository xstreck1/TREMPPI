#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file This file holds specifications of data types (typedefs and 3rd party classes) used by multiple classes throughout the computation.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <climits>
#include <cmath>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip> 
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <stdexcept>

#include <boost/range/algorithm.hpp>
#include <boost/range/counting_range.hpp>
#include <boost/program_options.hpp>
// Boost c++11 problem fix
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS
#define BOOST_PYTHON_STATIC_LIB 
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/python.hpp>

#include "configure.h"

const std::string VERSION("1.0.3.0");

using namespace std;

using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::is_any_of;
using boost::split;
using boost::trim;

namespace bpo = boost::program_options;
namespace rng = boost::range;
namespace bfs = boost::filesystem;
namespace alg = boost::algorithm;
namespace blg = boost::log;
namespace src = boost::log::sources;
namespace snk = boost::log::sinks;
namespace kwd = boost::log::keywords;
namespace bpy = boost::python;

const string NETWORK_FILENAME = "editor.json";
const string DATABASE_FILENAME = "database.sqlite";
const string PROPERTIES_FILENAME = "properties.json";
const string SELECTION_FILENAME = "select.json";
const string SUMMARY_FILENAME = "summary.json";
const string QUALITATIVE_FILENAME = "occurence.json";

// MODULES
int tremppi_correlate(int argc, char ** argv);
int tremppi_browse(int argc, char ** argv);
int tremppi_express(int argc, char ** argv);
int tremppi_init(int argc, char ** argv);
int tremppi_interact(int argc, char ** argv);
int tremppi_qualitative(int argc, char ** argv);
int tremppi_spawn(int argc, char ** argv);
int tremppi_summary(int argc, char ** argv);
int tremppi_update(int argc, char ** argv);
int tremppi_validate(int argc, char ** argv);
int tremppi_witness(int argc, char ** argv);
