/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

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
// Boost c++11 problem fix
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS
#define BOOST_PYTHON_STATIC_LIB 
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/python.hpp>

#include "configure.h"

// For BioSystems
const std::string VERSION("1.0.0");

using namespace std;

using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::is_any_of;
using boost::split;
using boost::trim;
using boost::algorithm::join;

namespace rng = boost::range;
namespace bfs = boost::filesystem;
namespace alg = boost::algorithm;
namespace bpy = boost::python;

const string DATA_FOLDER = "data";
const string PROJECTS_FOLDER = "projects";
const string NETWORK_FILENAME = "editor.json";
const string DATABASE_FILENAME = "database.sqlite";
const string PROPERTIES_FILENAME = "properties.json";
const string SELECTION_FILENAME = "select.json";
const string SUMMARY_FILENAME = "summary.json";
const string QUALITATIVE_FILENAME = "qualitative.json";
const string LOG_FILENAME = "log.txt";

// MODULES
int tremppi_bias(int argc, char ** argv);
int tremppi_browse(int argc, char ** argv);
int tremppi_clean(int argc, char ** argv);
int tremppi_cost(int argc, char ** argv);
int tremppi_configure(int argc, char ** argv);
int tremppi_correlations(int argc, char ** argv);
int tremppi_express(int argc, char ** argv);
int tremppi_finalize(int argc, char ** argv);
int tremppi_init(int argc, char ** argv);
int tremppi_impact(int argc, char ** argv);
int tremppi_sign(int argc, char ** argv);
int tremppi_qualitative(int argc, char ** argv);
int tremppi_quantitative(int argc, char ** argv);
int tremppi_regulations(int argc, char ** argv);
int tremppi_spawn(int argc, char ** argv);
int tremppi_update(int argc, char ** argv);
int tremppi_trace(int argc, char ** argv);
int tremppi_robustness(int argc, char ** argv);
int tremppi_witness(int argc, char ** argv);
