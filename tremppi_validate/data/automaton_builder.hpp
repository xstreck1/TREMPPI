/*
 * Copyright (C) 2012-2013 - Adam Streck
 * This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
 * ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
 * ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
 */

#include <tremppi_common/network/data_info.hpp>
#include "../data/property_info.hpp"
#include "automaton_structure.hpp"

 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ///  \brief Transform graph of the automaton into a set of labeled transitions in an AutomatonStructure object.
 ///
 /// This builder creates a basic automaton controlling property - this automaton is based on the AutomatonInterface.
 /// Automaton is provided with string labels on the edges that are parsed and resolved for the graph.
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace AutomatonBuilder {
	// Compute allowed values from string of constrains
	Configurations makeStateConst(const map<string, ActRange> & state_constraint, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names, const bool negate);

	//
	vector<PathCons> makePathConst(const map<string, PathCons>& constraints_list, const vector<string> & names);

	// Create the transitions from the model and fill the automaton with them.
	AutomatonStructure buildAutomaton(const PropertyInfo & property_info, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names);
};
