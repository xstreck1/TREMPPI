#pragma once

#include "../header.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INPUT/OUTPUT
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// What stream to use in output functions.
enum StreamType {error_str, results_str, verbose_str};

/// Symbol that separates values of a single parametrizations on output.
const std::string separator = ":";

/// Definition of naming abbreviations for edge labels:
namespace Label {
   const std::string Activating = "Activating";
   const std::string ActivatingOnly = "ActivatingOnly";
   const std::string Inhibiting = "Inhibiting";
   const std::string InhibitingOnly = "InhibitingOnly";
   const std::string NotActivating = "NotActivating";
   const std::string NotInhibiting = "NotInhibiting";
   const std::string Observable = "Observable";
   const std::string NotObservable = "NotObservable";
   const std::string Free = "Free";
   const vector<std::string> All = { Activating, ActivatingOnly, Inhibiting, InhibitingOnly, NotActivating, NotInhibiting, Observable, NotObservable, Free };
}

const std::string MODEL_SUFFIX = ".pmf";
const std::string PROPERTY_SUFFIX = ".ppf";
const std::string DATABASE_SUFFIX = ".sqlite";
const std::string OUTPUT_SUFFIX = ".out";
const std::string PLA_SUFFIX = ".pla";
const std::string GRAPH_SUFFIX = ".json";
const std::string FILTER_SUFFIX = ".filter";
const std::string NETWORK_SUFFIX = ".js";

const std::string COMPONENTS_TABLE = "Components";
const std::string REGULATIONS_TABLE = "Regulations";
const std::string PARAMETRIZATIONS_TABLE = "Parametrizations";
const vector<std::string> SOURCE_TYPES = { "select", "differ", "compare" };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DATA
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ID of a graph/automaton state.
typedef size_t StateID; 

/// ID of a graph component
typedef size_t CompID; 

/// Activity level of a specie.
typedef unsigned short ActLevel;

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

using namespace std;