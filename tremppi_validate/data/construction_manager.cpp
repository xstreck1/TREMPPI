#include "construction_manager.hpp"

tuple<Levels, Levels, Levels> ConstructionManager::getBounds(const RegInfos & reg_infos, const PropertyInfo & property_info) {
	tuple<Levels, Levels, Levels> result(tuple<Levels, Levels, Levels>{ Levels(reg_infos.size(), INF), Levels(reg_infos.size(), INF), Levels(reg_infos.size(), INF) });
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

	transform(WHOLE(get<0>(result)), begin(get<2>(result)), begin(get<2>(result)), [](const ActLevel min, const ActLevel max) {return static_cast<ActLevel>(max - min + 1); });
	return result;
}

ProductStructure ConstructionManager::construct(const RegInfos & reg_infos, const PropertyInfo & property_info) {
	const auto bounds = ConstructionManager::getBounds(reg_infos, property_info);
	vector<string> components(reg_infos.size(), "");

	// Create the UKS
	UnparametrizedStructure unparametrized_structure = UnparametrizedStructureBuilder::buildStructure(reg_infos, bounds);

	// Create the Buchi automaton
	transform(WHOLE(reg_infos), begin(components), [](const RegInfo & reg_info) {return reg_info.name; });
	AutomatonStructure automaton = AutomatonBuilder::buildAutomaton(property_info, bounds, components);

	// Create the product
	ProductBuilder product_builder;
	ProductStructure product = product_builder.buildProduct(move(unparametrized_structure), move(automaton));
	return product;
}