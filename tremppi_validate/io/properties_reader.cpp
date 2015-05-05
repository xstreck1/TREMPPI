#include "properties_reader.hpp"

pair<ActLevel, ActLevel> PropertiesReader::readBoundary(const string & value, const string & property_name, const string & comp_name) {
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
	return bound;
}

PathCons PropertiesReader::getTransitionConstraint(const string & constraint, const string & comp_name) {
	if (constraint == "stay") {
		return PathCons::pc_stay;
	}
	else if (constraint == "up") {
		return PathCons::pc_up;
	}
	else if (constraint == "down") {
		return PathCons::pc_down;
	}
	else {
		return PathCons::pc_none;
	}
}

vector<string> getComponent(const Json::Value & properties) {
	vector<string> result;

	for (const Json::Value & component : properties["components"]) {
		result.emplace_back(component.asString());
	}

	sort(WHOLE(result));
	return result;
}

vector<PropertyInfo> PropertiesReader::jsonToProperties(Json::Value & properties) {
	vector<PropertyInfo> automata;
	const vector<string> components = getComponent(properties);
	for (const Json::Value & property_node : properties) {
		PropertyInfo property_info;

		// Skip those that are not in use
		if (!property_node["validate"].asBool()) {
			continue;
		}

		property_info.robustness = property_node["robustness"].asBool();
		property_info.witness = property_node["witness"].asBool();

		property_info.name = property_node["name"].asString();
		property_info.ending = property_node["ending"].asString();

		try {
			property_info.bound = property_node["bound"].asInt();
		}
		catch (exception & e) {
			property_info.bound = INF;
		}

		// Get the experiment bounds
		for (const string & component : components) {
			const string value = property_node[component].asString();
			if (!value.empty()) {
				pair<ActLevel, ActLevel> component_bound = readBoundary(value, property_info.name, component);
				property_info.bounds.insert({ component, component_bound });
			}
		}

		// Parse records
		map<string, PathCons> last_trans;
		for (const const Json::Value & record : property_node["records"]) {
			map<string, ActRange> state_consts;
			map<string, PathCons> path_consts;

			// get the constraint
			for (const string & component : components) {
				if (record.isMember(component + "_value") && !record[component + "_value"].asString().empty()) {
					const string state_str = record[component + "_value"].asString();
					state_consts.insert(make_pair(component,  PropertiesReader::readBoundary(state_str, property_info.name, component)));
				}

				if (record.isMember(component + "_delta") && !record[component + "_delta"].asString().empty()) {
					const string trans_str = record[component + "_delta"].asString();
					path_consts.insert(make_pair(component,  PropertiesReader::getTransitionConstraint(trans_str, component)));
				}
			}


			const StateID ID = property_info.states.size();
			property_info.states.emplace_back(PropertyInfo::State{ property_info.states.size(), move(state_consts), move(path_consts) });
		}

		automata.emplace_back(move(property_info));
	}
	return automata;
}