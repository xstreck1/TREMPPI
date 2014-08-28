/*
 * Copyright (C) 2012-2013 - Adam Streck
 * This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
 * ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
 * ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
 */

#include <tremppi_common/network/model_translators.hpp>
#include "../model/property_automaton.hpp"
#include "automaton_structure.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  \brief Transform graph of the automaton into a set of labeled transitions in an AutomatonStructure object.
///
/// This builder creates a basic automaton controlling property - this automaton is based on the AutomatonInterface.
/// Automaton is provided with string labels on the edges that are parsed and resolved for the graph.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AutomatonBuilder {
	const Model & model; ///< Model that holds the data.
	const PropertyAutomaton & property;

	vector<string> names; ///< Name of the i-th specie.
	Levels maxes; ///< Maximal activity levels of the species.
	Levels mins; ///< Minimal activity levels of the species.
	Levels range_size; ///< Number of valid values for the species.

	/**
	 * Compute a vector of maximal levels and store information about states.
	 */
	void computeBoundaries() {
		// Compute naive bounds.
		for (const CompID ID : cscope(model.components)) {
			// Maximal values of species
			names.push_back(model.components[ID].name);
			maxes.push_back(model.components[ID].max_activity);
			mins.push_back(0);
		}

		// Add experiment constraints.
		ConstraintParser * cons_pars = new ConstraintParser(model.components.size(), ModelTranslators::getMaxLevel(model));
		cons_pars->addBoundaries(maxes, true);
		cons_pars->applyFormula(ModelTranslators::getAllNames(model), property.getExperiment());
		cons_pars->status();

		// Compute refined boundaries.
		mins = cons_pars->getBounds(false);
		maxes = cons_pars->getBounds(true);
		rng::transform(maxes, mins, back_inserter(range_size), [](const ActLevel max, const ActLevel min) {
			return max - min + 1;
		});
	}

	/**
	 * Creates transitions from labelled edges of BA and passes them to the automaton structure.
	 */
	void addTransitions(AutomatonStructure & automaton, const StateID ID) const {
		const PropertyAutomaton::Edges & edges = property.getEdges(ID);

		// Transform each edge into transition and pass it to the automaton
		for (const PropertyAutomaton::Edge & edge : edges) {
			// Compute allowed values from string of constrains
			ConstraintParser * parser = new ConstraintParser(maxes.size(), *max_element(maxes.begin(), maxes.end()));
			parser->applyFormula(names, edge.cons.values);
			parser->addBoundaries(maxes, true);
			parser->addBoundaries(mins, false);

			automaton.addTransition(ID, { edge.target_ID, parser, edge.cons.transient, edge.cons.stable });
		}
	}

	/**
	 * @brief setAutType sets type of the automaton based on the type of the property
	 */
	void setAutType(AutomatonStructure & automaton) {
		switch (property.getPropType()) {
		case LTL:
			automaton.my_type = BA_standard;
			break;
		case TimeSeries:
			automaton.my_type = BA_finite;
			break;
		default:
			throw runtime_error("Type of the verification automaton is not known.");
		}
	}

public:
	AutomatonBuilder(const Model & _model, const PropertyAutomaton & _property) : model(_model), property(_property) {
		computeBoundaries();
	}

	/**
	 * Create the transitions from the model and fill the automaton with them.
	 */
	AutomatonStructure buildAutomaton() {
		AutomatonStructure automaton;
		setAutType(automaton);
		const size_t state_count = property.getStatesCount();
		size_t state_no = 0;

		// List throught all the automaton states
		for (StateID ID = 0; ID < property.getStatesCount(); ID++) {
			// Fill auxiliary data
			automaton.addState(ID, property.isFinal(ID));
			// Add transitions for this state
			addTransitions(automaton, ID);
		}

		return automaton;
	}
};
