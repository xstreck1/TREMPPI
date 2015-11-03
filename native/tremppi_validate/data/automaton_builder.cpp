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

void AutomatonBuilder::buildSequence(const PropertyInfo & property_info, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names, AutomatonStructure & automaton, const size_t N, const size_t M)
{
	for (const StateID s_ID : crange(N)) {
		{
			MeasurementsCons initial_cons;
			for (const size_t m_i : crange(s_ID + 1u))
			{
				initial_cons.emplace_back(make_pair(makeStateConst(property_info.measurements[m_i].state_constraints, bounds, names), true));
			}
			if (s_ID + 1 != N)
			{
				initial_cons.emplace_back(make_pair(makeStateConst(property_info.measurements[s_ID + 1].state_constraints, bounds, names), false));
			}
			MeasurementsCons accepting_cons;
			for (const size_t m_i : crange(s_ID + 1u, M)) {
				accepting_cons.emplace_back(make_pair(makeStateConst(property_info.measurements[m_i].state_constraints, bounds, names), true));
			}
			automaton._states.emplace_back(AutState(s_ID, initial_cons, accepting_cons));
		}

		// Add transitions to the succesors
		for (const StateID t_ID : crange(s_ID, N)) {
			MeasurementsCons measurements_cons;
			for (const size_t m_i : crange(s_ID + 1, t_ID + 1))
			{
				measurements_cons.emplace_back(make_pair(makeStateConst(property_info.measurements[m_i].state_constraints, bounds, names), true));
			}
			if (t_ID + 1 != N)
			{
				measurements_cons.emplace_back(make_pair(makeStateConst(property_info.measurements[t_ID + 1].state_constraints, bounds, names), false));
			}

			DeltasCons deltas_cons;
			deltas_cons = makePathConst(property_info.measurements[t_ID].path_constraints, names);

			automaton._states[s_ID]._transitions.emplace_back(AutTransitionion(t_ID, measurements_cons, deltas_cons));
		}
	}
}

/*
*/
void AutomatonBuilder::buildCyclic(const PropertyInfo & property_info, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names, AutomatonStructure & automaton)
{
	const size_t N = property_info.measurements.size();
	for (const StateID s_ID : crange(N)) {
		{
			MeasurementsCons initial_cons;
			for (const size_t m_i : crange(s_ID + 1))
			{
				initial_cons.emplace_back(make_pair(makeStateConst(property_info.measurements[m_i].state_constraints, bounds, names), true));
			}
			if (s_ID + 1 != N)
			{
				initial_cons.emplace_back(make_pair(makeStateConst(property_info.measurements[s_ID + 1].state_constraints, bounds, names), false));
			}
			automaton._states.emplace_back(AutState(s_ID, initial_cons, initial_cons));
		}

		// Add transitions to the succesors
		for (const StateID shift : crange(N + 1)) {
			MeasurementsCons measurements_cons;
			for (const size_t m_i : crange(s_ID, s_ID + shift))
			{
				measurements_cons.emplace_back(make_pair(makeStateConst(property_info.measurements[m_i % N].state_constraints, bounds, names), true));
			}
			if (shift != N)
			{
				measurements_cons.emplace_back(make_pair(makeStateConst(property_info.measurements[(s_ID + shift) % N].state_constraints, bounds, names), false));
			}

			DeltasCons deltas_cons;
			deltas_cons = makePathConst(property_info.measurements[(s_ID + shift - 1) % N].path_constraints, names);

			automaton._states[s_ID]._transitions.emplace_back(AutTransitionion((s_ID + shift) % N, measurements_cons, deltas_cons));
		}
	}
}

void AutomatonBuilder::buildAutomaton(const PropertyInfo & property_info, const tuple<Levels, Levels, Levels> & bounds, const vector<string> & names, AutomatonStructure & automaton)
{
	if (property_info.measurements.empty()) {
		throw runtime_error("At least one measurement required, none found in " + property_info.name + ".");
	}

	if (property_info.ending == "open")
	{
		automaton._aut_type = BA_finite;
		// For 1 measurement only the open optimization is not possible
		const size_t N = max(1u, static_cast<size_t>(property_info.measurements.size() - 1u));
		buildSequence(property_info, bounds, names, automaton, N, property_info.measurements.size());
	}
	else if (property_info.ending == "stable")
	{
		automaton._aut_type = BA_stable;
		const size_t N = property_info.measurements.size();
		buildSequence(property_info, bounds, names, automaton, N, N);
	}
	else if (property_info.ending == "cyclic")
	{
		automaton._aut_type = BA_standard;
		buildCyclic(property_info, bounds, names, automaton);
	}
	else
	{
		throw runtime_error("Unknown property type " + property_info.ending + " of the property " + property_info.name);
	}
}
