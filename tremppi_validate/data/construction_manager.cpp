#include "construction_manager.hpp"

ProductStructure ConstructionManager::construct(const RegInfos & reg_infos, const PropertyInfo & property_info) {
	// Create the Buchi automaton
	AutomatonBuilder automaton_builder(reg_infos, property_info);
	AutomatonStructure automaton = automaton_builder.buildAutomaton();

	// Create the UKS
	UnparametrizedStructureBuilder unparametrized_structure_builder(reg_infos, property_info);
	UnparametrizedStructure unparametrized_structure = unparametrized_structure_builder.buildStructure();

	// Create the product
	ProductBuilder product_builder;
	ProductStructure product = product_builder.buildProduct(move(unparametrized_structure), move(automaton));
	return product;
}

void ConstructionManager::test(const RegInfos & reg_infos, const PropertyInfo & property_info)
{
	AutomatonBuilder automaton_builder(reg_infos, property_info);
	AutomatonStructure automaton = automaton_builder.buildAutomaton();
}
