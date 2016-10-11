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

#include "../data/model_translators.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Parses contrints as specified in the model and return a formula built from these constraints.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ConstraintFomatter 
{
	/* Context may contain specie-characters or number or colon or comma*/
	static bool belongsToContext(const char ch);


	static string addBrackets(string expr);

	/* Take the original constraint and replace contexts specified with their canonic version. */
	static string formatConstraint(string original, const Model & model, const CompID ID);

public:
	// Reformulate explicit user constraint so it matches the format of the constraint parser
	static string consToFormula(const Model & model, const CompID ID);
};