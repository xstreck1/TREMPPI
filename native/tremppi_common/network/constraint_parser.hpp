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

#include "../general/common_functions.hpp"
#include "definitions.hpp"

#include <gecode/kernel.hh>
#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/minimodel.hh>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief A class that accepts and parser constraints from a string formula and provides solutions to these constraints.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ConstraintParser : public Gecode::Space 
{
	Gecode::IntVarArray allowed_vals; ///< The actual values

	/* Transform the string into an integer, if possible. Return true iff sucessful. */
	bool getNumber(string atom_part, int & value);

	/* Find the number of the integer variable that corresponds to the given name of the specie. */
	size_t findName(const vector<string> & names, string specie_name);

	/* Apply the respective operator on the operands that are either converted to constants or matched to variables by name. */
	Gecode::LinIntRel applyOperator(const vector<string> & names, const string left_side, const string right_side, Gecode::IntRelType oper);

	/* Convert the atomic expression to the relevant constraint */
	Gecode::BoolExpr convertAtom(const vector<string> & names, const string & atom);

	/* Split the formula by the specified operator (either | or &). This is an intelligent split - only symbols that are outside parenthesis are considered. */
	vector<string> splitByOperator(const bool is_or, const string & formula);

	/* Trim the current formula and if it is enclosed in parenthesis, remove them. */
	void remove_parenthesis(string & formula);

	/*  */
	Gecode::BoolExpr resolveFormula(const vector<string> & names, string formula);

public:
	// For safety, do not allow any other sort of creating the parser than by dedicated functions
	ConstraintParser() = delete;
	ConstraintParser(ConstraintParser &&) = delete;
	ConstraintParser& operator=(ConstraintParser &&) = delete;
	ConstraintParser(const ConstraintParser &) = delete;
	ConstraintParser& operator=(const ConstraintParser &) = delete;
	ConstraintParser(const size_t number, const size_t upper_bound);
	ConstraintParser(bool share, ConstraintParser &other_space);

	virtual Gecode::Space *copy(bool share);

	void addBoundaries(const Levels & boundaries, const bool is_upper);

	/* Take a logical formula and make it into a constraint that gets propagated. */
	void applyFormula(const vector<string> & names, string formula);

	// print solution
	void print(void) const;

	// get boundaries after propagation
	Levels getBounds(bool upper) const;

	/* Obtain a vector with the solution */
	Levels getSolution() const;

	// True iff the required result is in between solution of the formula
	static bool contains(const vector<string> & names, const ActLevel max, const Levels & required, const string & formula);

	// True iff the required result is in between solution of the formula
	static bool contains(const vector<string> & names, const Levels & maxes, const Levels & required, const string & formula);
};