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

#include "constraint_formatter.hpp"

/* Context may contain specie-characters or number or colon or comma*/
bool ConstraintFomatter::belongsToContext(const char ch)
{
	return isalpha(ch) || isdigit(ch) != 0 || ch == ':' || ch == ',' || ch == '_';
}


string ConstraintFomatter::addBrackets(string expr)
{
	return string("(" + expr + ")");
}

/* Take the original constraint and replace contexts specified with their canonic version. */
string ConstraintFomatter::formatConstraint(string original, const Model & model, const CompID ID)
{
	string result;

	// Copy all the characters, replace contexts for canonic versions
	size_t start = INF;

	for (const size_t pos : cscope(original))
	{
		if (start == INF)
		{
			if (isalpha(original[pos]))
			{
				start = pos;
			}

			else
			{
				result.push_back(original[pos]);
			}
		}

		else
		{
			if (!belongsToContext(original[pos])) {
				string context = original.substr(start, pos - start);
				result.append(ModelTranslators::makeCanonic(model, context, ID));

				result.push_back(original[pos]);
				start = INF;
			}
		}
	}

	// Add the last context in the canonic form
	if (start != INF)
	{
		string context = original.substr(start);
		result.append(ModelTranslators::makeCanonic(model, context, ID));
	}

	return result;
}

// Reformulate explicit user constraint so it matches the format of the constraint parser
string ConstraintFomatter::consToFormula(const Model & model, const CompID ID)
{
	string formula; // Resulting formula specifying all the constraints

	formula = "tt";

	for (const string constraint : model.components[ID].constraints)
	{
		try
		{
			formula.append(" & " + addBrackets(formatConstraint(constraint, model, ID)));
		}
		catch (exception & e)
		{
			throw runtime_error("Error while parsing the constraint: " + quote(constraint) + ". " + e.what());
		}
	}

	return addBrackets(formula);
}