#include "automaton_builder.hpp"

ConstraintParser *  AutomatonBuilder::constrToParser(const string & constraint) const {
	ConstraintParser * parser = new ConstraintParser(maxes.size(), *max_element(maxes.begin(), maxes.end()));
	parser->applyFormula(names, constraint);
	parser->addBoundaries(maxes, true);
	parser->addBoundaries(mins, false);
	return parser;
}

void AutomatonBuilder::addTransitions(AutomatonStructure & automaton, const StateID ID) const {
	const PropertyAutomaton::Edges & edges = property_automaton.states[ID].edges;

	// Transform each edge into transition and pass it to the automaton
	for (const PropertyAutomaton::Edge & edge : edges) {
		automaton.addTransition(ID, { edge.target_ID, constrToParser(edge.constraint) });
	}
}

void AutomatonBuilder::setAutType(AutomatonStructure & automaton) {
	if (property_automaton.prop_type == "series")
		automaton.my_type = BA_finite;
	else if (property_automaton.prop_type == "stable")
		automaton.my_type = BA_stable;
	else if (property_automaton.prop_type == "cycle")
		automaton.my_type = BA_standard;
	else
		throw runtime_error("Type of the verification automaton is not known.");
}

vector<CompID> AutomatonBuilder::transformStables(const vector<string>& stable_names) {
	vector<CompID> stables(stable_names.size());

	transform(WHOLE(stable_names), begin(stables), [this](const string & stable_name) {
		return DataInfo::getCompID(reg_infos, stable_name);
	});

	return stables;
}

AutomatonBuilder::AutomatonBuilder(const RegInfos & _reg_infos, const PropertyAutomaton & _property_automaton) : reg_infos(_reg_infos), property_automaton(_property_automaton) {
	pair<Levels, Levels> bounds = PropertyHelper::getBounds(reg_infos, property_automaton);
	mins = bounds.first; maxes = bounds.second;
	rng::transform(maxes, mins, back_inserter(range_size), [](const ActLevel max, const ActLevel min) {return max - min + 1; });

	for (const CompID ID : cscope(reg_infos)) {
		names.push_back(reg_infos[ID].name);
	}
}

AutomatonStructure AutomatonBuilder::buildAutomaton() {
	AutomatonStructure automaton;
	setAutType(automaton);
	const size_t state_count = property_automaton.states.size();
	size_t state_no = 0;

	automaton.init_constr = AutomatonBuilder::constrToParser(property_automaton.init_condition);
	automaton.acc_constr = AutomatonBuilder::constrToParser(property_automaton.acc_condition);

	// List throught all the automaton states
	for (StateID ID : cscope(property_automaton.states)) {
		// Convert names into IDs
		const vector<string> & stable_names = property_automaton.states[ID].stables_list;
		vector<CompID> stables;
		stables = transformStables(stable_names);

		// Fill auxiliary data
		automaton.addState(ID, property_automaton.states[ID].final, stables);
		// Add transitions for this state
		addTransitions(automaton, ID);
	}

	return automaton;
}
