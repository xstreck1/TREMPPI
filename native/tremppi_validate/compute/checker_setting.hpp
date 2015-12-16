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

#pragma once

#include "../data/product_structure.hpp"
#include "../data/property_info.hpp"

class CheckerSetting 
{
public:
   vector<StateID> initial_states;
   vector<StateID> final_states;
   BoundType bound_type;
   size_t bfs_bound;
   bool circ; ///< True if we are looking only for cycles (i.e. init==accepting)
   bool stable; ///< True if we are looking only for paths with a stable last state.
   size_t minimal_count;

   CheckerSetting();

   bool isInitial(const StateID ID, const ProductStructure & product) const;

   bool isFinal(const StateID ID, const ProductStructure & product) const;

   const vector<StateID> getInitials(const ProductStructure & product) const;

   const vector<StateID> getFinals(const ProductStructure & product) const;
};

