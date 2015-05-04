#include "automaton_builder.hpp"

Configurations AutomatonBuilder::makeStateConst(const map<string, ActRange> & state_constraint, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names, const bool negate) {
	Configurations result(names.size());

	for (const CompID ID : cscope(names)) {
		Levels accepted;
		if (state_constraint.count(names[ID]) == 0) {
			accepted = vrange(get<0>(bounds)[ID], get<1>(bounds)[ID]);
		}
		else {
			const ActRange constr_range = state_constraint.at(names[ID]);
			if (negate) {
				for (const ActLevel act_level : crange(get<0>(bounds)[ID], get<1>(bounds)[ID])) {
					if (act_level < get<0>(bounds)[ID] || act_level > get<1>(bounds)[ID]) {
						accepted.emplace_back(act_level);
					}
				}
			} else {
				for (const ActLevel act_level : crange(get<0>(bounds)[ID], get<1>(bounds)[ID])) {
					if (act_level >= get<0>(bounds)[ID] && act_level <= get<1>(bounds)[ID]) {
						accepted.emplace_back(act_level);
					}
				}
			} 
		}
		result.emplace_back(move(accepted));
	}

	return result;
}


vector<PathCons> AutomatonBuilder::makePathConst(const map<string, PathCons>& constraints_list, const vector<string> & names) {
	vector<PathCons> result;

	for (const string & name : names) {
		if (constraints_list.count(name) > 0) {
			result.push_back(constraints_list.at(name));
		}
		else {
			result.push_back(PathCons::none);
		}
	}

	return result;
}

AutomatonStructure AutomatonBuilder::buildAutomaton(const PropertyInfo & property_info, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names) {
	if (property_info.ending == "any") {
		AutomatonStructure automaton(BA_finite, 
			makeStateConst(property_info.states.front().state_constraints, bounds, names, false), 
			makeStateConst(property_info.states.back().state_constraints, bounds, names, false));
		for (const StateID ID : crange(1u, property_info.states.size())) {
			const bool initial = ID == 1; 
			const bool final = ID == property_info.states.size() - 1;
			automaton.addState(ID - 1, initial, final);
			automaton.addTransition(ID - 1, ID - 1,
				makeStateConst(property_info.states[ID].state_constraints, bounds, names, true),
				makePathConst(property_info.states[ID - 1].path_constraints, names));
			if (!final) {
				automaton.addTransition(ID - 1, ID,
					makeStateConst(property_info.states[ID].state_constraints, bounds, names, false),
					makePathConst(property_info.states[ID].path_constraints, names));
			}
		}
		return automaton;
	}
	else if (property_info.ending == "stable") {
		AutomatonStructure automaton(BA_stable, {}, {});
		return automaton;
	}
	else if (property_info.ending.substr(0,4) == "goto") {
		AutomatonStructure automaton(BA_standard, {}, {});
		return automaton;
	}
	else {
		throw runtime_error("Unknown property type " + property_info.ending + " of the property " + property_info.name);
		return AutomatonStructure(BA_standard, {}, {});
	}
}
