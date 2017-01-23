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

#include "model_reader.hpp"

namespace ModelReader 
{
	// Convert model in JSON to Model object
	Model jsonToModel(const Json::Value & elements) 
	{
		Model model;

		CompID ID = 0;
		vector<string> names;
		for (const Json::Value node : elements["nodes"])
			names.emplace_back(node["data"]["Name"].asString());
		sort(WHOLE(names));

		string last_name = "";
		map<string, string> id_to_name;

		model.use_normalized = elements.get("use_normalized", true).asBool();

		for (const Json::Value node : elements["nodes"]) 
		{
			Model::ModelComp specie;
			
			specie.name = node["data"]["Name"].asString();
			specie.ID = distance(begin(names), find(WHOLE(names), specie.name));

			id_to_name.insert({ node["data"]["id"].asString(), specie.name });
			last_name = specie.name;

			specie.max_activity = node["data"]["MaxActivity"].asInt();

			specie.constraints.emplace_back(node["data"]["Constraint"].asString());

			model.components.emplace_back(move(specie));
		}

		sort(WHOLE(model.components), [](const Model::ModelComp & A, const Model::ModelComp & B) 
		{
			return A.ID < B.ID;
		});


		for (const Json::Value edge : elements["edges"]) 
		{
			const CompID source_id = ModelTranslators::findID(model, id_to_name[edge["data"]["source"].asString()]);
			const CompID target_id = ModelTranslators::findID(model, id_to_name[edge["data"]["target"].asString()]);


			Model::Regulation regulation
			{
				source_id,
				static_cast<ActLevel>(edge["data"]["Threshold"].asInt()),
				label_list.at(edge["data"]["Label"].asString())
			};

			model.components[target_id].regulations.emplace_back(move(regulation));
		}

		return model;
	}
}