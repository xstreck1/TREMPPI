#include "visit_storage.hpp"

VisitStorage::VisitStorage(const size_t state_count) {
	// Create states
	states.resize(state_count, INF);
	cost = 0;
}

void VisitStorage::reset() {
	states.assign(states.size(), INF);
	cost = 0;
}

bool VisitStorage::update(const StateID ID) {
	// If nothing is new return false
	if (isFound(ID))
		return false;
	// Add new parameters and return true
	states[ID] = cost;
	return true;
}

bool VisitStorage::isFound(const StateID ID) const {
	return states[ID] != INF;
}

size_t VisitStorage::getVisit(const StateID ID) const {
	return states[ID];
}

size_t VisitStorage::getCost() const {
	return cost;
}

void VisitStorage::incCost() {
	cost++;
}

bool VisitStorage::succeeded() const {
	return cost != INF;
}

void VisitStorage::notFound() {
	cost = INF;
}
