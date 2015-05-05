#include "product_structure.hpp"

ProdState::ProdState(const StateID ID, const StateID KS_ID, const StateID BA_ID, const bool is_initial, const bool is_final, const Levels & levels)
	: StateProperty(ID), AutomatonStateProperty(ID, is_initial, is_final), TSStateProperty<TSTransitionProperty>(ID, levels), _KS_ID(KS_ID), _BA_ID(BA_ID) {}

StateID ProductStructure::getProductID(const StateID KS_ID, const StateID BA_ID) const {
	return (BA_ID * _sizes.first + KS_ID);
}

const string ProductStructure::getString(const StateID ID) const {
	string label;

	for (const ActLevel lev : _states[ID]._levels)
		label += to_string(lev);

	label += char(_states[ID]._BA_ID + 65);

	return label;
}
