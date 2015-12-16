/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "visit_storage.hpp"

VisitStorage::VisitStorage(const size_t state_count) 
{
	// Create states
	states.resize(state_count, INF);
	cost = 0;
}


void VisitStorage::reset() 
{
	states.assign(states.size(), INF);
	cost = 0;
}


bool VisitStorage::update(const StateID ID) 
{
	// If nothing is new return false
	if (isFound(ID))
		return false;
	// Add new parameters and return true
	states[ID] = cost;
	return true;
}


bool VisitStorage::isFound(const StateID ID) const 
{
	return states[ID] != INF;
}


size_t VisitStorage::getVisit(const StateID ID) const 
{
	return states[ID];
}


size_t VisitStorage::getCost() const 
{
	return cost;
}


void VisitStorage::incCost() 
{
	cost++;
}


bool VisitStorage::succeeded() const 
{
	return cost != INF;
}


void VisitStorage::notFound() 
{
	cost = INF;
}
