#include "construction_manager.hpp"

ProductStructure ConstructionManager::construct(const RegInfos & reg_infos, const PropertyAutomaton & property_automaton) {
	// Create the Buchi automaton
	AutomatonBuilder automaton_builder(reg_infos, property_automaton);
	AutomatonStructure automaton = automaton_builder.buildAutomaton();

	// Create the UKS
	UnparametrizedStructureBuilder unparametrized_structure_builder(reg_infos, property_automaton);
	UnparametrizedStructure unparametrized_structure = unparametrized_structure_builder.buildStructure();

	// Create the product
	ProductBuilder product_builder;
	ProductStructure product = product_builder.buildProduct(move(unparametrized_structure), move(automaton));
	return product;
}

void ConstructionManager::test(const RegInfos & reg_infos, const PropertyAutomaton & property_automaton)
{
	AutomatonBuilder automaton_builder(reg_infos, property_automaton);
	AutomatonStructure automaton = automaton_builder.buildAutomaton();
}
