#include "product_builder.hpp"

void ProductBuilder::createSubspace(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, const StateID BA_ID, ProductStructure & product)
{
	for (const StateID KS_ID : cscope(structure)) 
	{
		StateID ID = product.computeID(KS_ID, BA_ID);
		const Levels& levels = structure._states[KS_ID]._levels;
		const bool initial = ProductBuilder::satisfiesMeasurements(levels, automaton._states[BA_ID]._initial_cons);
		const bool accepting = ProductBuilder::satisfiesMeasurements(levels, automaton._states[BA_ID]._accepting_cons);
		product._states.emplace_back(ProdState(ID, KS_ID, BA_ID, initial, accepting, levels));
	}
}
bool ProductBuilder::satisfiesDeltas(const UnparametrizedStructure & structure, const CompID s_ID, const CompID t_ID, const DeltasCons & deltas_cons)
{
	bool result = true;

	const Levels & s_levels = structure._states[s_ID]._levels;
	const Levels & t_levels = structure._states[t_ID]._levels;

	for (const CompID ID : cscope(deltas_cons))
	{
		switch (deltas_cons[ID]) {
		case PathCons::pc_up:
			result &= t_levels[ID] >= s_levels[ID];
			break;
		case PathCons::pc_down:
			result &= t_levels[ID] <= s_levels[ID];
			break;
		case PathCons::pc_stay:
			result &= t_levels[ID] == s_levels[ID];
			break;
		}
	}

	return result;
}
void ProductBuilder::addStateTransitions(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, const StateID s_BA_ID, const StateID s_KS_ID, const AutTransitionion & BA_transition, ProductStructure & product)
{
	StateID s_ID = product.computeID(s_KS_ID, s_BA_ID);

	// Add all the transient combinations for the kripke structure
	for (const size_t KS_trans_no : cscope(structure._states[s_KS_ID]))
	{
		const StateID t_KS_ID = structure._states[s_KS_ID]._transitions[KS_trans_no]._t_ID;
		if (satisfiesDeltas(structure, s_KS_ID, t_KS_ID, BA_transition._deltas_cons)) {
			const TransConst & trans_const = structure._states[s_KS_ID]._transitions[KS_trans_no]._trans_const;
			const StateID t_ID = product.computeID(t_KS_ID, BA_transition._t_ID);
			product._states[s_ID]._transitions.emplace_back(TSTransitionProperty(t_ID, trans_const));
		}
	}

	// Copy stay constraints
	product._states[s_ID]._stay_const = structure._states[s_KS_ID]._stay_const;

	// Add a self-loops
	product._states[s_ID]._loops.emplace_back(product.computeID(s_KS_ID, BA_transition._t_ID));
}

bool ProductBuilder::satisfiesMeasurements(const Levels & levels, const MeasurementsCons & measurements_cons)
{
	for (const MeasurementCons & measurement_cons : measurements_cons)
	{
		if (belongsToProduct(levels, measurement_cons.first) != measurement_cons.second)  // For each set, the set present must correspond to the condition
		{
			return false;
		}
	}
	return true;
}

void ProductBuilder::addSubspaceTransitions(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, const StateID s_BA_ID, const size_t BA_trans_no, ProductStructure & product)
{
	// List through the states that are allowed by the constraint
	const auto & BA_transition = automaton._states[s_BA_ID]._transitions[BA_trans_no];

	// Enumerate all states, for each go forward only if it does NOT meat the conditions
	for (const StateID s_KS_ID : cscope(structure._states)) 
	{
		if (satisfiesMeasurements(structure._states[s_KS_ID]._levels, BA_transition._measurements_cons))
		{
			addStateTransitions(structure, automaton, s_BA_ID, s_KS_ID, BA_transition, product);
		}
	}
}

void ProductBuilder::buildProduct(const UnparametrizedStructure & structure, const AutomatonStructure & automaton, ProductStructure & product)
{
	product._sizes = make_pair(structure.size(), automaton.size());
	product._aut_type = automaton._aut_type;

	// Creates states and their transitions
	for (const size_t BA_ID : cscope(automaton))
	{
		// Create that what relates to this BA state
		createSubspace(structure, automaton, BA_ID, product);
		for (const size_t trans_no : cscope(automaton._states[BA_ID]))
		{
			addSubspaceTransitions(structure, automaton, BA_ID, trans_no, product);
		}
	}
}