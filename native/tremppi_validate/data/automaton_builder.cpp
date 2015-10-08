#include "automaton_builder.hpp"


Configurations AutomatonBuilder::makeStateConst(const map<string, ActRange> & state_constraint, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names)
{
	Configurations result(names.size());

	for (const CompID ID : cscope(names))
	{
		if (state_constraint.count(names[ID]) == 0) {
			result[ID] = vrange(get<0>(bounds)[ID], static_cast<ActLevel>(get<1>(bounds)[ID] + 1));
		}
		else
		{
			const ActRange constr_range = state_constraint.at(names[ID]);

			for (const ActLevel act_level : crange(get<0>(bounds)[ID], static_cast<ActLevel>(get<1>(bounds)[ID] + 1)))
			{
				if (act_level >= constr_range.first && act_level <= constr_range.second) {
					result[ID].emplace_back(act_level);
				}
			}

		}
	}

	return result;
}


vector<PathCons> AutomatonBuilder::makePathConst(const map<string, PathCons>& constraints_list, const vector<string> & names)
{
	vector<PathCons> result;

	for (const string & name : names)
	{
		if (constraints_list.count(name) > 0) {
			result.push_back(constraints_list.at(name));
		}
		else
		{
			result.push_back(PathCons::pc_none);
		}
	}

	return result;
}



void AutomatonBuilder::buildTransient(const PropertyInfo & property_info, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names, AutomatonStructure & automaton)
{
	automaton._init_constr = makeStateConst(property_info.measurements.front().state_constraints, bounds, names);
	automaton._acc_constr = makeStateConst(property_info.measurements.back().state_constraints, bounds, names);

	if (property_info.measurements.size() == 1)
	{
		automaton._states.emplace_back(AutState(0, true, true));
		automaton._states[0]._transitions.emplace_back(AutTransitionion(0, makeStateConst({}, bounds, names), true, makePathConst({}, names)));
	}
	else
	{
		for (const StateID ID : crange(static_cast<size_t>(1), property_info.measurements.size())) {
			const bool initial = ID == 1;
			const bool final = ID == property_info.measurements.size() - 1;
			automaton._states.emplace_back(AutState(ID - 1, initial, final));
			// Add a loop with path contraints from the already satisfied measurement
			if (!final)
			{
				automaton._states[ID - 1]._transitions.emplace_back(
					AutTransitionion(
						ID - 1,
						makeStateConst(property_info.measurements[ID].state_constraints, bounds, names),
						false,
						makePathConst(property_info.measurements[ID - 1].path_constraints, names)
						));
				// Add a step to the next state under the current measurement and path condition
				automaton._states[ID - 1]._transitions.emplace_back(
					AutTransitionion(
						ID,
						makeStateConst(property_info.measurements[ID].state_constraints, bounds, names),
						true,
						makePathConst(property_info.measurements[ID].path_constraints, names)
						));
			}
			else {
				automaton._states[ID - 1]._transitions.emplace_back(AutTransitionion(ID - 1, makeStateConst({}, bounds, names), true, makePathConst({}, names)));
			}
		}
	}
}

/*
RULES:
	A is initial always
	X in goto X is the accepting, also the state where X is checked is accepting in the BA
	if goto X + 1 and X is the last measurement, all is accepting
	if goto A, then accepting=initial and it's placed on the check for B
*/

void AutomatonBuilder::buildCyclic(const PropertyInfo & property_info, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names, const char target, AutomatonStructure & automaton)
{
	const size_t target_ID = AutomatonStructure::NameToID(target);

	if (target_ID >= property_info.measurements.size())
	{
		throw runtime_error("Goto in the property " + property_info.name + " goes beyond the measurements.");
	}
	automaton._init_constr = makeStateConst(property_info.measurements.front().state_constraints, bounds, names);
	automaton._acc_constr = makeStateConst(property_info.measurements[target_ID].state_constraints, bounds, names);

	for (const StateID ID : crange(static_cast<size_t>(0), property_info.measurements.size()))
	{
		const bool initial = ID == 0u;
		const bool final = ID == target_ID;
		automaton._states.emplace_back(AutState(ID, initial, final));
		// Add a loop with path contraints from the already satisfied measurement
		automaton._states[ID]._transitions.emplace_back(AutTransitionion(
			ID,
			makeStateConst(property_info.measurements[ID].state_constraints, bounds, names),
			false,
			initial ? makePathConst({}, names) : makePathConst(property_info.measurements[ID - 1].path_constraints, names)
			));
		// Add a step to the next state under the current measurement and path condition, if this is last, then goto is the next
		automaton._states[ID]._transitions.emplace_back(AutTransitionion(
			property_info.measurements.size() == ID + 1 ? target_ID : ID + 1,
			makeStateConst(property_info.measurements[ID].state_constraints, bounds, names),
			true,
			makePathConst(property_info.measurements[ID].path_constraints, names)
			));
	}
}


void AutomatonBuilder::buildAutomaton(const PropertyInfo & property_info, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names, AutomatonStructure & automaton)
{
	if (property_info.measurements.empty()) {
		throw runtime_error("At least one measurement required, none found in " + property_info.name + ".");
	}

	if (property_info.ending == "any")
	{
		automaton._aut_type = BA_finite;
		buildTransient(property_info, bounds, names, automaton);
	}
	else if (property_info.ending == "stable")
	{
		automaton._aut_type = BA_stable;
		buildTransient(property_info, bounds, names, automaton);
	}
	else if (property_info.ending.substr(0, 4) == "goto")
	{
		automaton._aut_type = BA_standard;
		buildCyclic(property_info, bounds, names, property_info.ending[5], automaton);
	}
	else
	{
		throw runtime_error("Unknown property type " + property_info.ending + " of the property " + property_info.name);
	}
}
