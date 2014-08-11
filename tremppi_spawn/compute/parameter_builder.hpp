/*
* Copyright (C) 2012-2013 - Adam Streck
* This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
* ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
* ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
* For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
*/

#pragma once

#include "../kinetics/kinetics.hpp"
#include "../kinetics/parameter_helper.hpp"
#include "../model/model_helper.hpp"

class ParameterBuilder {
	/**
	* @brief getTargetValues  computes exact target values possible in given context.
	* @param autoreg index of the regulation that goes from itself
	*/
	static Levels getTargetValues(const Model & model, const map<SpecieID, Levels> & all_thrs, const Levels & thrs_comb, const size_t autoreg, const SpecieID t_ID) {
		Levels targets = vrange<ActLevel>(0u, model.species[t_ID].max_value + 1u);

		// If there is the loop restriction
		if (model.restrictions.bound_loop && autoreg != INF) {
			ActLevel self_thrs = thrs_comb[autoreg];
			Levels thresholds = (all_thrs.find(t_ID))->second;
			ActLevel bottom_border = 0u < self_thrs ? thresholds[self_thrs - 1] : 0u;
			ActLevel top_border = thresholds.size() > self_thrs ? thresholds[self_thrs] : model.species[t_ID].max_value + 1;
			Levels new_targets;

			// Add levels that are between the thresholds and one below/above if corresponds to the original.
			if (targets.front() < bottom_border)
				new_targets.push_back(bottom_border - 1);
			for (const auto target : targets)
				if (target >= bottom_border && target < top_border)
					new_targets.push_back(target);
			if (targets.back() >= top_border)
				new_targets.push_back(top_border);

			return new_targets;
		}

		return targets;
	}

	/**
	* @brief getSingleParam creates a parameter for a single context.
	* @return
	*/
	static Kinetics::Param addSingleParam(const Model & model, const map<SpecieID, Levels> & all_thrs, const Levels & thrs_comb, const SpecieID t_ID, const size_t autoreg_ID) {
		string context;
		map<StateID, Levels> requirements;

		// Loop over all the sources.
		for (auto source_num : crange(thrs_comb.size())) {
			// Find the source details and its current threshold
			string source_name = ModelTranslators::getRegulatorsNames(model, t_ID)[source_num];
			StateID s_ID = ModelTranslators::getRegulatorsIDs(model, t_ID)[source_num];
			auto thresholds = all_thrs.find(s_ID)->second;

			// Find activity level of the current threshold.
			ActLevel threshold = (thrs_comb[source_num] == 0) ? 0 : thresholds[thrs_comb[source_num] - 1];

			// Add current regulation as present.
			string regulation_name = source_name + ":" + to_string(threshold);

			// Add the regulation to the source
			context += regulation_name + ",";

			// Find in which levels the specie must be for the regulation to occur.
			ActLevel next_th = (thrs_comb[source_num] == thresholds.size()) ? model.species[s_ID].max_value + 1 : thresholds[thrs_comb[source_num]];

			requirements.insert(make_pair(s_ID, vrange(threshold, next_th)));
		}

		rng::for_each(requirements, [](pair<const StateID, Levels> & req){ rng::sort(req.second); });
		if (!context.empty())
			context.resize(context.length() - 1);
		return Kinetics::Param{ context, getTargetValues(model, all_thrs, thrs_comb, autoreg_ID, t_ID), move(requirements), Levels(), true };
	}

	// @brief createParameters Creates a description of kinetic parameters.
	static Kinetics::Params createParameters(const Model & model, const SpecieID t_ID) {
		Kinetics::Params result;

		auto all_thrs = ModelTranslators::getThresholds(model, t_ID);
		Levels bottom, thrs_comb, top;
		size_t autoreg{ INF };

		// These containers hold number of thresholds per regulator.
		for (auto & source_thresholds : all_thrs) {
			bottom.push_back(0); thrs_comb.push_back(0); top.push_back(source_thresholds.second.size());
			if (source_thresholds.first == t_ID)
				autoreg = thrs_comb.size() - 1;
		}

		// Loop over all the contexts.
		do {
			result.emplace_back(addSingleParam(model, all_thrs, thrs_comb, t_ID, autoreg));
		} while (iterate(top, bottom, thrs_comb));

		rng::sort(result, [](const Kinetics::Param & A, const Kinetics::Param & B){return A.context < B.context; });
		return result;
	}

public:
	// @brief fillParameters   fill idividual parameter values based on user specifications.
	static vector<Kinetics::Specie> buildParams(const Model & model) {
		vector<Kinetics::Specie>  result;

		// Create params for the non-input nodes
		for (const SpecieID ID : crange(model.species.size()))
			if (model.species[ID].spec_type != Model::Input)
				result.emplace_back(Kinetics::Specie{ model.species[ID].name, createParameters(model, ID), 0, 0 });
			else
				result.emplace_back(Kinetics::Specie{ model.species[ID].name, Kinetics::Params(), 0, 0 });

		return result;
	}
};

