/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "parameter_builder.hpp"

Kinetics::Param ParameterBuilder::addSingleParam(const Model & model, const map<CompID, Levels> & all_thrs, const Levels & thrs_comb, const CompID t_ID)
{
	string context;
	map<CompID, Levels> requirements;

	// Loop over all the sources.
	for (auto source_num : crange(thrs_comb.size()))
	{
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
		ActLevel next_th = (thrs_comb[source_num] == thresholds.size()) ? model.components[s_ID].max_activity + 1 : thresholds[thrs_comb[source_num]];

		requirements.insert(make_pair(s_ID, vrange(threshold, next_th)));
	}

	rng::for_each(requirements, [](pair<const StateID, Levels> & req) { rng::sort(req.second); });
	if (!context.empty())
		context.resize(context.length() - 1);

	Levels targets = vrange<ActLevel>(0u, model.components[t_ID].max_activity + 1u);
	return Kinetics::Param({ context, targets, requirements });
}

Kinetics::Params ParameterBuilder::createParameters(const Model & model, const CompID t_ID)
{
	Kinetics::Params result;

	auto all_thrs = ModelTranslators::getThresholds(model, t_ID);

	// These containers hold number of thresholds per regulator.
	Levels bottom, thrs_comb, top;

	for (auto & source_thresholds : all_thrs)
	{
		bottom.push_back(0);
		thrs_comb.push_back(0);
		top.push_back(source_thresholds.second.size());
	}

	// Loop over all the contexts.
	do
	{
		auto param = addSingleParam(model, all_thrs, thrs_comb, t_ID);
		result.emplace_back(move(param));
	} while (iterate(top, bottom, thrs_comb));

	sort(WHOLE(result), [](const Kinetics::Param & A, const Kinetics::Param & B) {return A.context < B.context; });
	return result;
}

vector<Kinetics::Component> ParameterBuilder::build(const Model & model)
{
	vector<Kinetics::Component> result;

	// Create params for the non-input nodes
	for (const CompID ID : crange(model.components.size()))
		result.emplace_back(Kinetics::Component({ ID, createParameters(model, ID), Configurations() }));

	return result;
}
