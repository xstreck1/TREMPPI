/*
 * Copyright (C) 2012-2013 - Adam Streck
 * This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
 * ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
 * ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
 */

#include <tremppi_common/network/data_info.hpp>
#include "../data/property_automaton.hpp"
#include "automaton_structure.hpp"

 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ///  \brief Transform graph of the automaton into a set of labeled transitions in an AutomatonStructure object.
 ///
 /// This builder creates a basic automaton controlling property - this automaton is based on the AutomatonInterface.
 /// Automaton is provided with string labels on the edges that are parsed and resolved for the graph.
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AutomatonBuilder {
	const RegInfos & reg_infos; ///< Model that holds the data.
	const PropertyAutomaton & property_automaton;

	vector<string> names; ///< Name of the i-th specie.
	Levels maxes; ///< Maximal activity levels of the species.
	Levels mins; ///< Minimal activity levels of the species.
	Levels range_size; ///< Number of valid values for the species.

	// Compute allowed values from string of constrains
	ConstraintParser * constrToParser(const string & constraint) const;

	/**
	 * Creates transitions from labelled edges of BA and passes them to the automaton structure.
	 */
	void addTransitions(AutomatonStructure & automaton, const StateID ID) const;

	/**
	 * @brief setAutType sets type of the automaton based on the type of the property_automaton
	 */
	void setAutType(AutomatonStructure & automaton);

	//
	vector<CompID> transformStables(const vector<string> & stable_names);

public:
	AutomatonBuilder(const RegInfos & _reg_infos, const PropertyAutomaton & _property_automaton);

	/**
	 * Create the transitions from the model and fill the automaton with them.
	 */
	AutomatonStructure buildAutomaton();
};
