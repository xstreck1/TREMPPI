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


#include "parametrizations_builder.hpp"

/**
* @brief isSubordinate returns true if the current context is the same as the compared context only with a higher activity value in specificed regulator.
*/
bool ParametrizationsBuilder::isSubordinate(const vector<Model::Regulation> &reguls, const Kinetics::Param &current, const Kinetics::Param &compare, const CompID source_ID)
{
	for (const Model::Regulation & regul : reguls) {
		// All the regulations must have the same requirements, except for the one with the specified source, which must connect on the value.

		if (regul.s_ID != source_ID)
		{
			if (current.requirements.find(regul.s_ID)->second.back() != compare.requirements.find(regul.s_ID)->second.back()) {
				return false;
			}
		}

		else if (current.requirements.find(regul.s_ID)->second.front() != compare.requirements.find(regul.s_ID)->second.back() + 1)
		{
			return false;
		}

	}
	return true;
}

/**
* Return true if the given parameter's context is dependent on the given regulation.
*/
bool ParametrizationsBuilder::containsRegulation(const Kinetics::Param &param_data, const Model::Regulation &regul)
{
	return param_data.requirements.find(regul.s_ID)->second.front() == regul.threshold;
}

/* initial constraining of the values to the predefined ones */
string ParametrizationsBuilder::addAllowed(const Levels &targets, const string & context)
{
	string result = "ff ";

	// Add only present values
	for (const size_t val : targets)
		result += " | " + to_string(val) + " = " + context;

	return result;
}

/* Create an expression based on the conditions obtained from the edge label */
string ParametrizationsBuilder::replaceInLabel(const string & label, const string & plus, const string & minus)
{
	string result = label;


	auto replace = [&result](const char symbol, const string & formula) -> void
	{
		size_t pos = result.find(symbol);

		while (pos != result.npos)
		{
			result.replace(pos, 1, formula);
			pos = result.find(symbol);
		}
	};

	replace('+', plus);
	replace('-', minus);

	return result;
}

/* For each regulation create a constraint corresponding to its label */
void ParametrizationsBuilder::createEdgeCons(const vector<Model::Regulation> & reguls, const Kinetics::Params & params, const Model::Regulation & regul, string & plus, string & minus)
{
	plus = minus = "ff ";

	for (const auto & param : params)
	{
		if (containsRegulation(param, regul)) {

			for (const auto & compare : params)
			{
				if (isSubordinate(reguls, param, compare, regul.s_ID)) {
					plus += " | " + param.context + " > " + compare.context;
					minus += " | " + param.context + " < " + compare.context;
				}
			}
		}
	}
}

void ParametrizationsBuilder::addBrackets(string & formula)
{
	formula = "(" + formula + ")";
}

string ParametrizationsBuilder::createFormula(const vector<Model::Regulation> & reguls, const Kinetics::Params & params)
{
	string result = "tt ";

	// Add constraints for all the regulations

	for (auto & regul : reguls)
	{
		string plus, minus, label;
		createEdgeCons(reguls, params, regul, plus, minus);
		addBrackets(plus);
		addBrackets(minus);
		label = replaceInLabel(regul.label, plus, minus);
		addBrackets(label);
		result += " & " + label;
	}

	// List all the possible target values for a parameter

	for (auto & param : params)
	{
		string allowed;
		allowed = addAllowed(param.targets, param.context);
		addBrackets(allowed);
		result += " & " + allowed;
	}

	return result;
}

/* Create constraint space on parametrizations for the given specie and enumerate and store all the solutions. */
Configurations ParametrizationsBuilder::createPartCol(const bool check_only, const Kinetics::Params & params, const string formula, const size_t max_value)
{
	Configurations result;

	// Build the space
	vector<string> names;
	for (const auto & param : params)
		names.push_back(param.context);

	ConstraintParser * cons_pars = new ConstraintParser(names.size(), max_value);

	// Impose constraints
	cons_pars->applyFormula(names, formula);

	// Enumerate if not check only

	if (!check_only)
	{
		// Conduct search
		Gecode::DFS<ConstraintParser> search(cons_pars);
		delete cons_pars;

		while (ConstraintParser *match = search.next())
		{
			Levels solution = match->getSolution();
			Levels shortened;
			result.push_back(solution);
			delete match;
		}
	}

	return result;
}

/**
* Entry function of parsing, tests and stores subcolors for all the components.
*/
void ParametrizationsBuilder::build(const bool check_only, const Model &model, Kinetics & kinetics)
{
	// Cycle through components

	for (CompID ind = model.components.size(); ind > 0; --ind)
	{
		CompID ID = ind - 1;

		// Solve the parametrizations
		string formula = createFormula(model.components[ID].regulations, kinetics.components[ID].params) + " & " + ConstraintFomatter::consToFormula(model, ID);
		kinetics.components[ID].subcolors = createPartCol(check_only, kinetics.components[ID].params, formula, model.components[ID].max_activity);
	}
}