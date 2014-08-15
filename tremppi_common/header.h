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
#include <boost/filesystem.hpp>
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

// The global setup for Tremppi apps - mainly sets paths to the executables and data
struct {
	string PROGRAM_NAME;
	bfs::path HOME_PATH;
	bfs::path WORK_PATH;

	void set(const char * name, const char * arg, const bfs::path & _WORK_PATH) {
		PROGRAM_NAME = name;
		WORK_PATH = _WORK_PATH;

		// Set the home
		char* home_path = getenv("TREMPPI_HOME");

		if (home_path != NULL)
			HOME_PATH = bfs::path{ home_path };
		else {
			bfs::path test_path{ arg };

			// Test for ../data to exist
			test_path = test_path.parent_path() /= bfs::path{ ".." };
			if (bfs::exists(test_path / bfs::path{ "data" })) 
				HOME_PATH = test_path;
			// Test for ../../data to exist - Windows only
			else {
				test_path = test_path /= bfs::path{ ".." };
				if (bfs::exists(test_path / bfs::path{ "data" }));
				HOME_PATH = test_path;
			}
		}
	}
} tremppi_system;