#include "construction_manager.hpp"

tuple<Levels, Levels, Levels> ConstructionManager::getBounds(const RegInfos & reg_infos, const PropertyInfo & property_info) {
	tuple<Levels, Levels, Levels> result(tuple<Levels, Levels, Levels>{ Levels(reg_infos.size()), Levels(reg_infos.size()), Levels(reg_infos.size()) });
	// Lower bound (min 0)
	transform(WHOLE(reg_infos), begin(get<0>(result)), [&property_info](const RegInfo & reg_info) {
		if (property_info.bounds.count(reg_info.name) > 0) {
			return property_info.bounds.at(reg_info.name).first;
		}
		else {
			return static_cast<ActLevel>(0);
		}
	});
	// Upper bound (max max_activity)
	transform(WHOLE(reg_infos), begin(get<1>(result)), [&property_info](const RegInfo & reg_info) {
		if (property_info.bounds.count(reg_info.name) > 0) {
			return property_info.bounds.at(reg_info.name).second;
		}
		else {
			return reg_info.max_activity;
		}
	});

	transform(WHOLE(get<0>(result)), begin(get<1>(result)), begin(get<2>(result)), [](const ActLevel min, const ActLevel max) {return static_cast<ActLevel>(max - min + 1); });
	return result;
}

void ConstructionManager::construct(const RegInfos & reg_infos, const PropertyInfo & property_info, ProductStructure & product) {
	const auto bounds = ConstructionManager::getBounds(reg_infos, property_info);
	vector<string> components(reg_infos.size());
	transform(WHOLE(reg_infos), begin(components), [](const RegInfo & reg_info) {return reg_info.name; });

	// Create the UKS
	UnparametrizedStructure unparametrized_structure;
	unparametrized_structure.setBounds(bounds);
	UnparametrizedStructureBuilder::buildStructure(reg_infos, unparametrized_structure);

	// Create the Buchi automaton
	AutomatonStructure automaton;
	AutomatonBuilder::buildAutomaton(property_info, bounds, components, automaton);

	// Create the product
	ProductBuilder::buildProduct(unparametrized_structure, automaton, product);
}