#include "properties_reader.hpp"

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

		// Get the bounds
		for (const RegInfo & reg_info : reg_infos) {
			pair<ActLevel, ActLevel> bound = { 0, INF };
			string value = property_node[reg_info.name].asString();
			std::smatch sm;
			// Bound is specified
			if (regex_match(value, sm, BOUNDARY_EXPR)) {
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
				throw invalid_argument("unknown value " + value + " for the component " + reg_info.name + " of the property " + automaton.name);
			}
			// Corrections on the bounds
			if (bound.first > bound.second)
				throw invalid_argument("invalid value " + value + " for the component " + reg_info.name + " of the property " + automaton.name);
			bound.second = std::min(reg_info.max_activity, bound.second);
			automaton.bounds.push_back(bound);
		}

		StateID ID = 0;

		// Add an accepting first state
		if (automaton.prop_type == "cycle") {
			automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, true,{}, PropertyAutomaton::Edges({ { 1, "tt" } }) });
			ID++;
		}

		size_t record_i = 0;
		vector<string> stables_list;
		for (const Json::Value & record : property_node["records"]) {
			string constraint;

			// get the constraint
			for (const RegInfo & reg_info : reg_infos) {
				string value = record[reg_info.name + "_value"].asString();
				std::smatch sm;
				if (regex_match(value, sm, BOUNDARY_EXPR)) {
					if (sm[1].str() == "(") {
						constraint += reg_info.name + ">" + sm[2].str() + "&";
					}
					else {
						constraint += reg_info.name + ">=" + sm[2].str() + "&";
					}
					if (sm[4].str() == ")") {
						constraint += reg_info.name + "<" + sm[3].str() + "&";
					}
					else {
						constraint += reg_info.name + "<=" + sm[3].str() + "&";
					}
				}
			}
			constraint.resize(std::max(0u, constraint.size() - 1));
			string negation = "!(" + constraint + ")";

			if (record_i == 0) {
				automaton.init_condition = constraint;
				if (automaton.prop_type == "cycle") {
					automaton.acc_condition = constraint;
				}
			}
			else if (record_i == property_node["records"].size() - 1 && automaton.prop_type != "cycle") {
				automaton.acc_condition = constraint;
			}
			else {
				automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, false, move(stables_list),{ { ID, negation },{ ID + 1, constraint } } });
				ID++;
			}

			// get the stables
			stables_list.clear();
			for (const RegInfo & reg_info : reg_infos) {
				if (record[reg_info.name + "_delta"].asString() == "stay") {
					stables_list.push_back(reg_info.name);
				}
			}
			record_i++;
		}

		// Loop to the first
		if (automaton.prop_type == "cycle") {
			automaton.states.back().edges.back().target_ID = 0;
		}
		// Add a new state to the end that has a loop and compies the requirement for stables
		if (automaton.prop_type == "series" || automaton.prop_type == "stable") {
			automaton.states.emplace_back(PropertyAutomaton::State{ to_string(ID), ID, true, move(stables_list),{ { ID, "tt" } } });
		}

		automata.emplace_back(move(automaton));
	}
	return automata;
}
