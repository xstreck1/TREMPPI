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

	/**
	 * Creates transitions from labelled edges of BA and passes them to the automaton structure.
	 */
	void addTransitions(AutomatonStructure & automaton, const StateID ID) const {
		const PropertyAutomaton::Edges & edges = property_automaton.states[ID].edges;

		// Transform each edge into transition and pass it to the automaton
		for (const PropertyAutomaton::Edge & edge : edges) {
			// Compute allowed values from string of constrains
			ConstraintParser * parser = new ConstraintParser(maxes.size(), *max_element(maxes.begin(), maxes.end()));
			parser->applyFormula(names, edge.constraint);
			parser->addBoundaries(maxes, true);
			parser->addBoundaries(mins, false);

			automaton.addTransition(ID, { edge.target_ID, parser });
		}
	}

	/**
	 * @brief setAutType sets type of the automaton based on the type of the property_automaton
	 */
	void setAutType(AutomatonStructure & automaton) {
		if (property_automaton.prop_type == "series")
			if (property_automaton.states.back().stables_list.empty())
				automaton.my_type = BA_finite;
			else // Partial stable states require full check
				automaton.my_type = BA_standard;
		else if (property_automaton.prop_type == "cycle")
			automaton.my_type = BA_standard;
		else
			throw runtime_error("Type of the verification automaton is not known.");
	}

	vector<CompID> transformStables(const vector<string> & stable_names) {
		vector<CompID> stables(stable_names.size());

		transform(WHOLE(stable_names), begin(stables), [this](const string & stable_name) {
			return DataInfo::getCompID(reg_infos, stable_name);
		});

		return stables;
	}

public:
	AutomatonBuilder(const RegInfos & _reg_infos, const PropertyAutomaton & _property_automaton) : reg_infos(_reg_infos), property_automaton(_property_automaton) {
		pair<Levels, Levels> bounds = PropertyHelper::getBounds(reg_infos, property_automaton);
		mins = bounds.first; maxes = bounds.second;
		rng::transform(maxes, mins, back_inserter(range_size), [](const ActLevel max, const ActLevel min) {return max - min + 1; });

		for (const CompID ID : cscope(reg_infos)) {
			names.push_back(reg_infos[ID].name);
		}
	}

	/**
	 * Create the transitions from the model and fill the automaton with them.
	 */
	AutomatonStructure buildAutomaton() {
		AutomatonStructure automaton;
		setAutType(automaton);
		const size_t state_count = property_automaton.states.size();
		size_t state_no = 0;

		// List throught all the automaton states
		for (StateID ID : cscope(property_automaton.states)) {
			// Convert names into IDs
			const vector<string> & stable_names = property_automaton.states[ID].stables_list;
			vector<CompID> stables;
			if (stable_names.size() == 1 && stable_names[0] == "#ALL")
				stables = transformStables(DataInfo::getAllNames(reg_infos));
			else
				stables = transformStables(stable_names);

			// Fill auxiliary data
			automaton.addState(ID, property_automaton.states[ID].final, stables);
			// Add transitions for this state
			addTransitions(automaton, ID);
		}

		return automaton;
	}
};
