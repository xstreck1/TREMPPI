#pragma once

#include "automaton_structure.hpp"
#include "unparametrized_structure.hpp"
#include "transition_system_interface.hpp"

/// Storing a single transition to neighbour state together with its transition function.
struct ProdTransitionion : public TransitionProperty {
	const TransConst & trans_const;

	ProdTransitionion(const StateID _target_ID, const TransConst & _trans_const);
};

/// State of the product - same as the state of UKS but put together with a BA state.
struct ProdState : public AutomatonStateProperty<ProdTransitionion> {
	const StateID KS_ID; ///< ID of an original KS state this one is built from
	const StateID BA_ID; ///< ID of an original BA state this one is built from
	const Levels & levels; ///< species_level[i] = activation level of specie i in this state
	vector<StateID> loops; ///< States with the Same KS ID, but different BA that are possible targets

	/// Simple filler, assigns values to all the variables
	ProdState(const StateID ID, const StateID _KS_ID, const StateID _BA_ID, const bool initial, const bool final, const Levels & _species_level);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Holds a product structure - the one that is used in coloring procedure.
///
/// This is the final step of construction - a structure that is acutally used during the computation. For simplicity, it copies data from its predecessors (BA and UKS).
/// @attention States of product are indexed as (BA_state_count * KS_state_ID + BA_state_ID) - e.g. if 3-state BA state ((1,0)x(1)) would be at position 3*1 + 1 = 4.
///
/// ProductStructure data can be set only from the ProductBuilder object.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ProductStructure : public AutomatonInterface<ProdState>, public TSInterface<ProdState> {
	friend class ProductBuilder;
	UnparametrizedStructure structure;
	AutomatonStructure automaton;

public:
	ProductStructure() = default;
	ProductStructure(UnparametrizedStructure _structure, AutomatonStructure _automaton);
	ProductStructure(ProductStructure &&) = default;
	ProductStructure(const ProductStructure &) = delete;
	ProductStructure& operator=(const ProductStructure &) = delete;
	ProductStructure& operator=(ProductStructure && other);

	const UnparametrizedStructure & getStructure() const;

	const AutomatonStructure & getAutomaton() const;

	StateID getProductID(const StateID KS_ID, const StateID BA_ID) const;

	StateID getBAID(const StateID ID) const;

	StateID getKSID(const StateID ID) const;

	const string getString(const StateID ID) const;

	const string getString(const StateID ID, const size_t cost) const;
};