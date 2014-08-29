#pragma once 

#include "../data/property_automaton.hpp"
#include "automaton_builder.hpp"
#include "unparametrized_structure_builder.hpp"
#include "product_builder.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief STEP 2 - Builds all the structures and stores them within a ConstructionHolder.
///
/// ConstructionManager overviews the whole process of construction of structures from information contained within a model file.
/// All the objects constructed are stored within a provided CostructionHolder and further acessible only via constant getters.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace ConstructionManager {
	/**
	 * Function that constructs all the data in a cascade of temporal builders.
	 */
	ProductStructure construct(const RegInfos & reg_infos, const PropertyAutomaton & property, const Kinetics & kinetics) {
		// Create the UKS
		UnparametrizedStructureBuilder unparametrized_structure_builder(reg_infos, property, kinetics);
		UnparametrizedStructure unparametrized_structure = unparametrized_structure_builder.buildStructure();

		// Create the Buchi automaton
		AutomatonBuilder automaton_builder(reg_infos, property);
		AutomatonStructure automaton = automaton_builder.buildAutomaton();

		// Create the product
		ProductBuilder product_builder;
		ProductStructure product = product_builder.buildProduct(move(unparametrized_structure), move(automaton));
		return product;
	}
}