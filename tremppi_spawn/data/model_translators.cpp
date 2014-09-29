#include "model_translators.hpp"

CompID ModelTranslators::findID(const Model & model, const string & name) {
	auto comp_it = find_if(WHOLE(model.components), [&name](const Model::ModelComp & component){
		return name == component.name;
	});
	return (comp_it != end(model.components)) ? comp_it->ID : INF;
}

vector<CompID> ModelTranslators::getRegulatorsIDs(const Model & model, const CompID ID) {
	set<CompID> IDs;
	for (auto regul : model.components[ID].regulations) {
		IDs.insert(regul.s_ID);
	}
	return vector<CompID>(IDs.begin(), IDs.end());
}

vector<string> ModelTranslators::getRegulatorsNames(const Model & model, const CompID ID) {
	auto regulators = getRegulatorsIDs(model, ID);
	vector<string> names;
	for (auto reg : regulators) {
		names.push_back(model.components[reg].name);
	}
	return names;
}

map<CompID, Levels > ModelTranslators::getThresholds(const Model & model, const CompID ID) {
	map<CompID, Levels> thresholds;
	for (auto reg : model.components[ID].regulations) {
		auto key = thresholds.find(reg.s_ID);
		if (key == thresholds.end()) {
			thresholds.insert(make_pair(reg.s_ID, Levels(1, reg.threshold)));
		}
		else {
			key->second.push_back(reg.threshold);
		}
	}

	for (auto & ths : thresholds) {
		std::sort(WHOLE(ths.second));
	}

	return thresholds;
}

ActLevel ModelTranslators::getThreshold(const Model & model, const string & context, const CompID t_ID, const string & name, const size_t pos) {
	// Regulator not present.
	if (pos == context.npos)
		return 0;
	const size_t COLON_POS = pos + name.length(); // Where colon is supposed to be.
	auto thresholds = getThresholds(model, t_ID).find(findID(model, name))->second;

	// Regulator level not specified.
	if (context[COLON_POS] != ':') {
		// Control if the context is unambiguous.
		if (thresholds.size() > 1)
			throw runtime_error("Ambiguous context " + quote(context) + " - no threshold specified for a regulator " + quote(name) + " that has multiple regulations.");
		// If valid, add the threshold 1.
		return thresholds[0];
	}

	// There is not a threshold given after double colon.
	if (context[COLON_POS] == ':' && (COLON_POS == (context.npos - 1) || !isdigit(context[COLON_POS + 1])))
		throw runtime_error("No threshold given after colon in the context " + quote(context) + " of the regulator " + name);

	// Add a threshold if uniquely specified.
	string to_return;
	size_t number = 1;
	// Copy all the numbers
	while (isdigit(context[COLON_POS + number])) {
		to_return.push_back(context[COLON_POS + number]);
		number++;
	}
	// Check if the threshold is valid
	size_t thrs = boost::lexical_cast<size_t>(to_return);
	if (thrs != 0 && find(thresholds.begin(), thresholds.end(), thrs) == thresholds.end())
		throw runtime_error("The threshold value \"" + to_return + "\" is not valid for the context " + quote(context) + ".");

	return thrs;
}

string ModelTranslators::makeCanonic(const Model & model, const string & context, const CompID t_ID) {
	string new_context; // new canonic form
	const auto names = getRegulatorsNames(model, t_ID);

	// Control correctness
	vector<string> reguls;
	split(reguls, context, is_any_of(","));
	for (const string regul : reguls) {
		string comp_name = regul.substr(0, min(regul.find(':'), regul.size()));
		if (find(names.begin(), names.end(), comp_name) == names.end())
			throw runtime_error("The component " + quote(comp_name) + " is not a regulator of " + quote(model.components[t_ID].name));
	}

	// For each of the regulator of the component.
	for (const auto & name : names) {
		auto pos = context.find(name);
		ActLevel threshold = getThreshold(model, context, t_ID, name, pos);
		new_context += name + ":" + to_string(threshold) + ",";
	}

	// Remove the last comma and return
	return new_context.substr(0, new_context.length() - 1);
}

const Model::Regulation & ModelTranslators::findRegulation(const Model & model, const CompID t_ID, const CompID s_ID, const ActLevel threshold) {
	const auto & reguls = model.components[t_ID].regulations;
	for (const Model::Regulation & regul : reguls)
		if (regul.s_ID == s_ID && regul.threshold == threshold)
			return regul;
	throw runtime_error("Failed to match the regulation " + to_string(s_ID) + " -" + to_string(threshold) + "-> " + to_string(t_ID));
}