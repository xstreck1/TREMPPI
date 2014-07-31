#pragma once

#include <algorithm>
#include <climits>
#include <cmath>
#include <chrono>
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

using namespace std;

const string VERSION("1.0.3.0");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file This file holds specifications of data types (typedefs and 3rd party classes) used by multiple classes throughout the computation.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INPUT/OUTPUT
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// What stream to use in output functions.
enum StreamType {error_str, results_str, verbose_str};

/// Symbol that separates values of a single parametrizations on output.
const string separator = ":";

/// Definition of naming abbreviations for edge labels:
namespace Label {
   const string Activating = "Activating";
   const string ActivatingOnly = "ActivatingOnly";
   const string Inhibiting = "Inhibiting";
   const string InhibitingOnly = "InhibitingOnly";
   const string NotActivating = "NotActivating";
   const string NotInhibiting = "NotInhibiting";
   const string Observable = "Observable";
   const string NotObservable = "NotObservable";
   const string Free = "Free";
}

const string MODEL_SUFFIX = ".pmf";
const string PROPERTY_SUFFIX = ".ppf";
const string DATABASE_SUFFIX = ".sqlite";
const string OUTPUT_SUFFIX = ".out";
const string PLA_SUFFIX = ".pla";
const string GRAPH_SUFFIX = ".json";
const string FILTER_SUFFIX = ".filter";

const string COMPONENTS_TABLE = "Components";
const string REGULATIONS_TABLE = "Regulations";
const string PARAMETRIZATIONS_TABLE = "Parametrizations";
const vector<string> SOURCE_TYPES = { "select", "differ", "compare" };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DATA
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ID of a graph/automaton state.
typedef size_t StateID; 

/// ID of a graph component
typedef size_t CompID; 

/// Activity level of a specie.
typedef short ActLevel;
#define stoal(X) boost::lexical_cast<ActLevel, string>(X)

/// Vector of activation levels of species used for labelling of states of KS.
typedef vector<ActLevel> Levels;

/// Transitional values in a state of BA.
typedef vector<Levels> Configurations;

/// Index of the color - may be a big number.
typedef size_t ParamNo;

/// Parametrizations in Neighbourhood
typedef vector<ParamNo> Neighbours;

/// Lattice of neighbourhood interactions
using NeighLattice = vector<Neighbours>;

/// this value represents infinite value - used for showing that a variable is unset
const size_t INF = ~0u;
const ActLevel INF_SHORT = ~((ActLevel) 0u);

// Post Algebra types
using PVal = int;
using PLit = vector<PVal>; ///< Post's algebra literal
using PMin = vector<PLit>; ///< Post's algebra minterm
using PDNF = vector<PMin>; ///< Post's algebra disjunctive normal form

