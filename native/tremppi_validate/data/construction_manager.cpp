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

#include "construction_manager.hpp"

tuple<Levels, Levels, Levels> ConstructionManager::getBounds(const RegInfos & reg_infos, const PropertyInfo & property_info) 
{
	tuple<Levels, Levels, Levels> result(tuple<Levels, Levels, Levels>{ Levels(reg_infos.size()), Levels(reg_infos.size()), Levels(reg_infos.size()) });
	// Lower bound (min 0)

	transform(WHOLE(reg_infos), begin(get<0>(result)), [&property_info](const RegInfo & reg_info) 
	{
		if (property_info.bounds.count(reg_info.name) > 0) 
		{
			return property_info.bounds.at(reg_info.name).first;
		}

		else 
		{
			return static_cast<ActLevel>(0);
		}
	});
	// Upper bound (max max_activity)

	transform(WHOLE(reg_infos), begin(get<1>(result)), [&property_info](const RegInfo & reg_info) 
	{
		if (property_info.bounds.count(reg_info.name) > 0) 
		{
			return property_info.bounds.at(reg_info.name).second;
		}

		else 
		{
			return reg_info.max_activity;
		}
	});

	transform(WHOLE(get<0>(result)), begin(get<1>(result)), begin(get<2>(result)), [](const ActLevel min, const ActLevel max) {return static_cast<ActLevel>(max - min + 1); });
	return result;
}


void ConstructionManager::construct(const RegInfos & reg_infos, const PropertyInfo & property_info, ProductStructure & product) 
{
	UnparametrizedStructure unparametrized_structure;
	AutomatonStructure automaton;

	unparametrized_structure._bounds = ConstructionManager::getBounds(reg_infos, property_info);
	vector<string> components(reg_infos.size());
	transform(WHOLE(reg_infos), begin(components), [](const RegInfo & reg_info) {return reg_info.name; });

	// Create the UKS
	UnparametrizedStructureBuilder::buildStructure(reg_infos, unparametrized_structure);

	// Create the Buchi automaton
	AutomatonBuilder::buildAutomaton(property_info, unparametrized_structure._bounds, components, automaton);

	// Create the product
	ProductBuilder::buildProduct(unparametrized_structure, automaton, product);
}

void ConstructionManager::restrictProperties(const RegInfos & reg_infos, const PropertyInfo & property_info, Levels & parametrization) 
{
	for (const RegInfo & reg_info : reg_infos)
	{
		if (property_info.bounds.count(reg_info.name) > 0)
		{
			for (const pair<size_t, Configurations > & requirement : reg_info.requirements)
			{
				if (parametrization[requirement.first] < property_info.bounds.at(reg_info.name).first)
				{
					parametrization[requirement.first] = property_info.bounds.at(reg_info.name).first;
				}
				if (parametrization[requirement.first] > property_info.bounds.at(reg_info.name).second)
				{
					parametrization[requirement.first] = property_info.bounds.at(reg_info.name).second;
				}
			}
		}
	}
}