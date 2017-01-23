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

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/definitions.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Storage for data parsed from the model.
///
/// Model stores model data in the raw form, almost the same as in the model file itself.
/// Model data can be set only form the ModelParser object.
/// Rest of the code can access the data only via constant getters - once the data are parse, model remains constant.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Model 
{
	NO_COPY(Model);

	struct ModelComp;

	struct Regulation 
	{
		CompID s_ID; ///< A reference to the source component.
		ActLevel threshold; ///< Level of the regulator required for the regulation to be active.
		string label; ///< An edge semantics label, if provided.
	};
	typedef vector<Regulation> Regulations;

	/// Structure that holds data about a single component. Most of the data is equal to that in the model file.
	struct ModelComp 
	{
		CompID ID; ///< Numerical id, lexicographically ordered.
		string name; ///< Actuall name of the specie.
		ActLevel max_activity; ///< Maximal activation level of the specie.

		Regulations regulations; ///< Regulations of the specie (activations or inhibitions by other components).

		vector<string> constraints; ///< A set of constraints on the component, if provided.
	};

	string name; ///< Model identifier.
	vector<ModelComp> components; ///< Vector of all components of the model.

	bool use_normalized; ///< Is the model to be normalized?
};
