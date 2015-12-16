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

#include "automaton_structure.hpp"

AutTransitionion::AutTransitionion(const StateID t_ID, const MeasurementsCons measurements_cons, const DeltasCons deltas_cons)
	: TransitionProperty(t_ID), _measurements_cons(move(measurements_cons)), _deltas_cons(move(deltas_cons))
{}

AutState::AutState(const StateID ID, const MeasurementsCons initial_cons, const MeasurementsCons accepting_cons)
	: StateProperty(ID), AutomatonStateProperty(ID, true, true), _initial_cons(move(initial_cons)), _accepting_cons(move(accepting_cons))
{}

char AutomatonStructure::IDToName(const size_t ID) 
{
	return static_cast<char>(ID + 65);
}

size_t AutomatonStructure::NameToID(const char name) 
{
	return static_cast<size_t>(name) - 65u;
}

