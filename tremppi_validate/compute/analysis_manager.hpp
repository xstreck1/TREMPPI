#pragma once

#include "../data/property_info.hpp"

#include "witness_searcher.hpp"
#include "visit_storage.hpp"
#include "model_checker.hpp"
#include "robustness_compute.hpp"
#include "checker_setting.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief STEP 3 - Control class for the computation.
///
/// Manager of the analysis procedure - takes the reference data constructed during previous steps and computes and executes the analysis.
/// Analysis is done in three steps:
///	-# preparation: empties data and starts a new round.
///   -# analysis: computes the coloring, stored in the storage object and adds data to coloring analyzer if needed.
///   -# conclusion: stores additional data and outputs
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AnalysisManager {
	const ProductStructure & product;
	unique_ptr<VisitStorage> storage; ///< Class that holds.
	unique_ptr<WitnessSearcher> searcher; ///< Class to build wintesses.
	unique_ptr<RobustnessCompute> computer; ///< Class to compute robustness.
	const AutType aut_type;
	const size_t bfs_bound;
	const bool witness;
	const bool robustness; 
	
	/**
	 * @brief standard conduct model check for a lasso
	 */
	tuple<size_t, multimap<StateID, StateID>, double >  standard(const Levels & parametrization);

	/**
	 * @brief finite conduct model check with reachability only
	 */
	tuple<size_t, multimap<StateID, StateID>, double > finite(const Levels & parametrization, const bool stable);

public:
	/**
	 * Constructor builds all the data objects that are used within.
	 */
	AnalysisManager(const ProductStructure & _product, const size_t _bfs_bound, const bool _witness, const bool _robustness);

	/**
	 * The entry point of analysis
	 */
	tuple<size_t, multimap<StateID, StateID>, double >  check(const Levels & parametrization);
};