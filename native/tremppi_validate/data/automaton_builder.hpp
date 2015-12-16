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

#include <tremppi_common/network/data_info.hpp>
#include "../data/property_info.hpp"
#include "automaton_structure.hpp"
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ///  \brief Transform graph of the automaton into a set of labeled transitions in an AutomatonStructure object.
 ///
 /// This builder creates a basic automaton controlling property - this automaton is based on the AutomatonInterface.
 /// Automaton is provided with string labels on the edges that are parsed and resolved for the graph.
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace AutomatonBuilder 
{
	// Compute allowed values from string of constrains
	Configurations makeStateConst(const map<string, ActRange> & state_constraint, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names);
	//
	vector<PathCons> makePathConst(const map<string, PathCons>& constraints_list, const vector<string> & names);
	//
	void buildSequence(const PropertyInfo & property_info, const tuple<Levels, Levels, Levels>& bounds, const vector<string>& names, AutomatonStructure & automaton, const size_t N, const size_t M);
	//
	void buildCyclic(const PropertyInfo & property_info, const tuple<Levels, Levels, Levels>& bounds, const vector<string>& names, AutomatonStructure & automaton);
	// Create the transitions from the model and fill the automaton with them.
	void buildAutomaton(const PropertyInfo & property_info, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names, AutomatonStructure & automaton);
}
