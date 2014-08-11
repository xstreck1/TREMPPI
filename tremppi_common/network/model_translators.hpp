/*
 * Copyright (C) 2012-2013 - Adam Streck
 * This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
 * ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
 * ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
 */

#pragma once

#include "definitions.hpp"
#include "model.hpp"

namespace ModelTranslators {
	/**
	 * @brief findID  obtain ID of the specie.
	 */
	CompID findID(const Model & model, const string & name) {
		for (const CompID ID : cscope(model.species))
			if (model.species[ID].name == name)
				return ID;
		return INF;
	}

	/**
	 * @return	unique IDs of regulators of the specie
	 */
	vector<CompID> getRegulatorsIDs(const Model & model, const CompID ID) {
		set<CompID> IDs;
		for (auto regul : model.species[ID].regulations) {
			IDs.insert(regul.source);
		}
		return vector<CompID>(IDs.begin(), IDs.end());
	}

	/**
	 * @return	names of the regulators of the specie
	 */
	vector<string> getRegulatorsNames(const Model & model, const CompID ID) {
		auto regulators = getRegulatorsIDs(model, ID);
		vector<string> names;
		for (auto reg : regulators) {
			names.push_back(model.species[reg].name);
		}
		return names;
	}

	/**
	* @return	names of all the species
	*/
	vector<string> getAllNames(const Model & model) {
		vector<string> names;
		for (const Model::ModelSpecie & specie : model.species)
			names.push_back(specie.name);
		return names;
	}

	/**
	 * @brief getThresholds Finds a list of thresholds for each regulator of a given component.
	 * @param ID
	 * @return
	 */
	map<CompID, Levels > getThresholds(const Model & model, const CompID ID) {
		map<CompID, Levels> thresholds;
		for (auto reg : model.species[ID].regulations) {
			auto key = thresholds.find(reg.source);
			if (key == thresholds.end()) {
				thresholds.insert(make_pair(reg.source, Levels(1, reg.threshold)));
			}
			else {
				key->second.push_back(reg.threshold);
			}
		}

		for (auto ths : thresholds) {
			sort(ths.second.begin(), ths.second.end());
		}

		return thresholds;
	}

	/**
	 * @brief getThreshold  For a given regulator, find out what it's threshold in the given context is.
	 * @return  threshold value in the given context
	 */
	ActLevel getThreshold(const Model & model, const string & context, const CompID t_ID, const string & name, const size_t pos) {
		// Regulator not present.
		if (pos == context.npos)
			return 0;
		const size_t COLON_POS = pos + name.length(); // Where colon is supposed to be.
		auto thresholds = getThresholds(model, t_ID).find(findID(model, name))->second;

		// Regulator level not specified.
		if (context[COLON_POS] != ':') {
			// Control if the context is unambiguous.
			if (thresholds.size() > 1)
				throw runtime_error("Ambiguous context \"" + context + "\" - no threshold specified for a regulator " + name + " that has multiple regulations.");
			// If valid, add the threshold 1.
			return thresholds[0];
		}

		// There is not a threshold given after double colon.
		if (context[COLON_POS] == ':' && (COLON_POS == (context.npos - 1) || !isdigit(context[COLON_POS + 1])))
			throw runtime_error("No threshold given after colon in the context \"" + context + "\" of the regulator " + name);

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
			throw runtime_error("The threshold value \"" + to_return + "\" is not valid for the context \"" + context + "\".");

		return thrs;
	}

	/**
	 * @brief formCanonicContext   Transforms the regulation specification into a canonic form (\forall r \in regulator [r:threshold,...]).
	 * @param context any valid context form as a string
	 * @return canonic context form
	 */
	static string makeCanonic(const Model & model, const string & context, const CompID t_ID) {
		string new_context; // new canonic form
		const auto names = getRegulatorsNames(model, t_ID);

		// Control correctness
		vector<string> reguls;
		split(reguls, context, is_any_of(","));
		for (const string regul : reguls) {
			string spec_name = regul.substr(0, min(regul.find(':'), regul.size()));
			if (find(names.begin(), names.end(), spec_name) == names.end())
				throw runtime_error("Unrecognized specie \"" + spec_name + "\" in the context \"" + context + "\".");
		}

		// For each of the regulator of the specie.
		for (const auto & name : names) {
			auto pos = context.find(name);
			ActLevel threshold = getThreshold(model, context, t_ID, name, pos);
			new_context += name + ":" + to_string(threshold) + ",";
		}

		// Remove the last comma and return
		return new_context.substr(0, new_context.length() - 1);
	}

	// @return regulation with given parameters
	const Model::Regulation & findRegulation(const Model & model, const CompID t_ID, const CompID s_ID, const ActLevel threshold) {
		const auto & reguls = model.species[t_ID].regulations;
		for (const Model::Regulation & regul : reguls)
			if (regul.source == s_ID && regul.threshold == threshold)
				return regul;
		throw runtime_error("Failed to match the regulation " + to_string(s_ID) + " -" + to_string(threshold) + "-> " + to_string(t_ID));
	}

	// @return the maximal level in between the species
	const ActLevel getMaxLevel(const Model & model) {
		return rng::max_element(model.species, [](const Model::ModelSpecie & A, const Model::ModelSpecie & B) {
			return A.max_activity < B.max_activity;
		})->max_value;
	}

	// @return canonic form of the label
	string readLabel(const string & label) {
		string formula;

		// Find the constrain and return its valuation
		if (label.compare(Label::Activating) == 0)
			formula = "+";
		else if (label.compare(Label::ActivatingOnly) == 0)
			formula = "(+ & !-)";
		else if (label.compare(Label::Inhibiting) == 0)
			formula = "-";
		else if (label.compare(Label::InhibitingOnly) == 0)
			formula = "(- & !+)";
		else if (label.compare(Label::NotActivating) == 0)
			formula = "!+";
		else if (label.compare(Label::NotInhibiting) == 0)
			formula = "!-";
		else if (label.compare(Label::Observable) == 0)
			formula = "(+ | -)";
		else if (label.compare(Label::NotObservable) == 0)
			formula = "(!+ & !-)";
		else if (label.compare(Label::Free) == 0)
			formula = "tt";
		else
			formula = label;

		return formula;
	}
}