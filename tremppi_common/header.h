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

const string NETWORK_FILENAME = "editor.js";
const string DATABASE_FILENAME = "database.sqlite";
const string PROPERTIES_FILENAME = "properties.js";
const string SELECTION_FILENAME = "select.js";

// The global setup for Tremppi apps - mainly sets paths to the executables and data
class TremppiSystem {
public:
	string PROGRAM_NAME; // Name of the program running
	bfs::path HOME_PATH; // The path to the home of tremppi
	bfs::path WORK_PATH; // The directory in which tremppi works currently
	bfs::path EXEC_PATH; // The path from where the program was executed
	bfs::path BIN_PATH; // The path to the executed binary
	bool standalone; // True if executing the standalone application instead of the main launcher

	//
	void set(const char * name, const char * arg, const string & _WORK_PATH);

	//
	template<typename OptionsT>
	bpo::variables_map TremppiSystem::initiate(const string & name, int argc, char **argv) {
		bpo::variables_map po;

		try {
			if (argc < 1)
				throw runtime_error("No parameters.");
			OptionsT options;
			po = options.parseProgramOptions(argc, argv);

			tremppi_system.set(name.c_str(), argv[0], po["path"].as<string>());
			logging.init();
			BOOST_LOG_TRIVIAL(info) << tremppi_system.PROGRAM_NAME << " started.";
		}
		catch (exception & e) {
			cerr << e.what() << endl;
			exit(1);
		}

		return po;
	}
};

extern TremppiSystem tremppi_system;

