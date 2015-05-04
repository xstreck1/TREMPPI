#pragma once 

#include "../data/property_info.hpp"
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
	// @return (Mins, Maxes, Range)
	tuple<Levels, Levels, Levels> getBounds(const RegInfos & reg_infos, const PropertyInfo & property_info);
	
	// Function that constructs all the data in a cascade of temporal builders.
	ProductStructure construct(const RegInfos & reg_infos, const PropertyInfo & property);
}