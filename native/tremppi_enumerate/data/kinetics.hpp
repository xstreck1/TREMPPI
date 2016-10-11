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

#include <tremppi_common/network/definitions.hpp>
#include <tremppi_common/general/common_functions.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Kinetic parameters and the corresponding parametrizations are stored in this structure.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Kinetics 
{
	NO_COPY(Kinetics);

    struct Param 
    {
		string context; ///< String representation of the context.
		Levels targets; ///< Towards which level this context may regulate.
		map<CompID, Levels> requirements; ///< Levels of the source components this param is relevant to, the levels are sorted.
	};
	using Params = vector < Param > ;

    struct Component 
    {
		CompID ID; ///< ID of the component, shared with the model
		Params params; ///< Vector of parameters, sorted lexicographically by the context.
		Configurations subcolors; ///< Subparametrizations of this particular component
	};

	vector<Component> components; ///< Species shared with the model, sorted lexicographically. 
};
