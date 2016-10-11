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

#include <tremppi_common/network/constraint_parser.hpp>

#include "../data/kinetics_translators.hpp"
#include "../io/constraint_formatter.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Class that computes feasible parametrizations for each specie from
/// the edge constrains and stores them in a ParametrizationHolder object.
///
/// This construction may be optimized by including the warm-start constraint satisfaction.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ParametrizationsBuilder 
{
	/**
	* @brief isSubordinate returns true if the current context is the same as the compared context only with a higher activity value in specificed regulator.
	*/
	static bool isSubordinate(const vector<Model::Regulation> &reguls, const Kinetics::Param &current, const Kinetics::Param &compare, const CompID source_ID);

	/**
	* Return true if the given parameter's context is dependent on the given regulation.
	*/
	static bool containsRegulation(const Kinetics::Param &param_data, const Model::Regulation &regul);

	/* initial constraining of the values to the predefined ones */
	static string addAllowed(const Levels &targets, const string & context);

	/* Create an expression based on the conditions obtained from the edge label */
	static string replaceInLabel(const string & label, const string & plus, const string & minus);

	/* For each regulation create a constraint corresponding to its label */
	static void createEdgeCons(const vector<Model::Regulation> & reguls, const Kinetics::Params & params, const Model::Regulation & regul, string & plus, string & minus);

	static void addBrackets(string & formula);

	static string createFormula(const vector<Model::Regulation> & reguls, const Kinetics::Params & params);

	/* Create constraint space on parametrizations for the given specie and enumerate and store all the solutions. */
	static Configurations  createPartCol(const bool check_only, const Kinetics::Params & params, const string formula, const size_t max_value);

public:
	/**
	* Entry function of parsing, tests and stores subcolors for all the components.
	*/
	static void build(const bool check_only, const Model &model, Kinetics & kinetics);
};