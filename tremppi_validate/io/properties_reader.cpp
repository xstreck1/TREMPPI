#include "properties_reader.hpp"

pair<ActLevel, ActLevel> PropertiesReader::readExperiment(const string & value, const string & property_name, const string & comp_name, const ActLevel max_activity) {
	pair<ActLevel, ActLevel> bound = { 0, INF };
	std::smatch sm;
	// Bound is specified
	if (regex_match(value, sm, INT_NUM)) {
		bound.first = bound.second = stoi(sm[0].str());
	}
	else if (regex_match(value, sm, INT_BOUNDARY)) {
		if (sm[1].str() == "(") {
			bound.first = stoi(sm[2].str()) + 1;
		}
		else {
			bound.first = stoi(sm[2].str());
		}
		if (sm[4].str() == ")") {
			bound.second = stoi(sm[3].str()) - 1;
		}
		else {
			bound.second = stoi(sm[3].str());
		}
	}
	else if (value != "") {
		throw invalid_argument("unknown value " + value + " for the component " + comp_name + " of the property " + property_name);
	}
	// Corrections on the bounds
	if (bound.first > bound.second)
		throw invalid_argument("invalid value " + value + " for the component " + comp_name + " of the property " + property_name);
	bound.second = std::min(max_activity, bound.second);
	return bound;
}

string PropertiesReader::getStateConstraint(const string & value, const string & comp_name) {
	std::smatch sm;
	if (regex_match(value, sm, INT_NUM)) {
		return comp_name + "=" + sm[0].str() + "&";
	}
	else if (regex_match(value, sm, INT_BOUNDARY)) {
		if (sm[1].str() == "(") {
			return comp_name + ">" + sm[2].str() + "&";
		}
		else {
			return comp_name + ">=" + sm[2].str() + "&";
		}
		if (sm[4].str() == ")") {
			return comp_name + "<" + sm[3].str() + "&";
		}
		else {
			return comp_name + "<=" + sm[3].str() + "&";
		}
	}
	else return "";
}

pair<string, PathCons> PropertiesReader::getTransitionConstraint(const string & constraint, const string & comp_name) {
	if (constraint == "stay") {
		return make_pair(comp_name, PathCons::stay);
	}
	else if (constraint == "up") {
		return make_pair(comp_name, PathCons::up);
	}
	else if (constraint == "down") {
		return make_pair(comp_name, PathCons::down);
	}
	else {
		return make_pair(comp_name, PathCons::none);
	}
}

vector<PropertyAutomaton> PropertiesReader::jsonToProperties(const RegInfos & reg_infos, Json::Value & properties) {
	vector<PropertyAutomaton> automata;
	for (const Json::Value & property_node : properties) {
		PropertyAutomaton automaton;

		// Skip those that are not in use
		if (!property_node["validate"].asBool()) {
			continue;
		}

		automaton.robustness = property_node["robustness"].asBool();
		automaton.witness = property_node["witness"].asBool();

		automaton.name = property_node["name"].asString();
		automaton.prop_type = property_node["type"].asString();

		try {
			automaton.bound = property_node["bound"].asInt();
		}
		catch (exception & e) {
			automaton.bound = INF;
		}

		// Get the experiment bounds
		for (const RegInfo & reg_info : reg_infos) {
			string value = property_node[reg_info.name].asString();
			pair<ActLevel, ActLevel> component_bound = readExperiment(value, automaton.name, reg_info.name, reg_info.max_activity);
			automaton.bounds.push_back(component_bound);
		}

		StateID ID = 0;

		// Add an accepting first state
		if (automaton.prop_type == "cycle") {
			automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, true,{}, PropertyAutomaton::Edges({ { 1, "tt" } }) });
			ID++;
		}

		// Parse records
		size_t record_i = 0;
		map<string, PathCons> trans_consts;
		for (const Json::Value & record : property_node["records"]) {
			string state_constraint;

			// get the constraint
			for (const RegInfo & reg_info : reg_infos) {
				string value = record[reg_info.name + "_value"].asString();
				state_constraint += PropertiesReader::getStateConstraint(value, reg_info.name);
			}
			state_constraint.resize(std::max(0, static_cast<int>(state_constraint.size()) - 1));
			string negation = "!(" + state_constraint + ")";

			if (record_i == 0) {
				automaton.init_condition = state_constraint;
				if (automaton.prop_type == "cycle") {
					automaton.acc_condition = state_constraint;
				}
			}
			else if (record_i == property_node["records"].size() - 1 && automaton.prop_type != "cycle") {
				automaton.acc_condition = state_constraint;
			}
			else {
				automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, false, move(trans_consts),{ { ID, negation },{ ID + 1, state_constraint } } });
				ID++;
			}

			// get the constraints
			for (const RegInfo & reg_info : reg_infos) {
				const string constraint = record[reg_info.name + "_delta"].asString();
				trans_consts.insert(getTransitionConstraint(constraint, reg_info.name));
			}
			record_i++;
		}

		// Loop to the first
		if (automaton.prop_type == "cycle") {
			automaton.states.back().edges.back().target_ID = 0;
		}
		// Add a new state to the end that has a loop and compies the requirement for stables
		if (automaton.prop_type == "series" || automaton.prop_type == "stable") {
			automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, true, move(trans_consts),{ { ID, "tt" } } });
		}

		automata.emplace_back(move(automaton));
	}
	return automata;
}
