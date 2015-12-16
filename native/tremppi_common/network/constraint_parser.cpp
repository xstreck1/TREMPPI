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

#include "constraint_parser.hpp"

bool ConstraintParser::getNumber(string atom_part, int & value)
{
	try
	{
		value = lexical_cast<int>(atom_part);
	}

	catch (...)
	{
		return false;
	}
	return true;
}

size_t ConstraintParser::findName(const vector<string> & names, string specie_name)
{
	for (const size_t name_no : cscope(names))
		if (specie_name.compare(names[name_no]) == 0)
			return name_no;
	throw runtime_error("Unrecognized variable name \"" + specie_name + "\".");
}

Gecode::LinIntRel ConstraintParser::applyOperator(const vector<string> & names, const string left_side, const string right_side, Gecode::IntRelType oper)
{
	int left_val = 0, right_val = 0;
	if (getNumber(left_side, left_val))
		return Gecode::LinIntRel(left_val, oper, allowed_vals[findName(names, right_side)]);
	else if (getNumber(right_side, right_val))
		return Gecode::LinIntRel(allowed_vals[findName(names, left_side)], oper, right_val);
	else
		return Gecode::LinIntRel(allowed_vals[findName(names, left_side)], oper, allowed_vals[findName(names, right_side)]);
}

Gecode::BoolExpr ConstraintParser::convertAtom(const vector<string> & names, const string & atom)
{
	if ((atom.compare("tt") == 0) || atom.empty())
		return Gecode::BoolExpr(allowed_vals[0] == allowed_vals[0]);
	else if (atom.compare("ff") == 0)
		return Gecode::BoolExpr(allowed_vals[0] != allowed_vals[0]);
	else if (atom.find("<=") != atom.npos)
		return applyOperator(names, atom.substr(0, atom.find("<=")), atom.substr(atom.find("<=") + 2), Gecode::IntRelType::IRT_LQ);
	else if (atom.find(">=") != atom.npos)
		return applyOperator(names, atom.substr(0, atom.find(">=")), atom.substr(atom.find(">=") + 2), Gecode::IntRelType::IRT_GQ);
	else if (atom.find("!=") != atom.npos)
		return applyOperator(names, atom.substr(0, atom.find("!=")), atom.substr(atom.find("!=") + 2), Gecode::IntRelType::IRT_NQ);
	else if (atom.find("=") != atom.npos)
		return applyOperator(names, atom.substr(0, atom.find("=")), atom.substr(atom.find("=") + 1), Gecode::IntRelType::IRT_EQ);
	else if (atom.find("<") != atom.npos)
		return applyOperator(names, atom.substr(0, atom.find("<")), atom.substr(atom.find("<") + 1), Gecode::IntRelType::IRT_LE);
	else if (atom.find(">") != atom.npos)
		return applyOperator(names, atom.substr(0, atom.find(">")), atom.substr(atom.find(">") + 1), Gecode::IntRelType::IRT_GR);
	else
		return Gecode::LinIntRel(allowed_vals[findName(names, atom)] == 1);
}

vector<string> ConstraintParser::splitByOperator(const bool is_or, const string & formula)
{
	vector<string> result;

	int parity = 0;
	size_t last_pos = 0;

	for (const size_t pos : cscope(formula))
	{
		if (formula[pos] == '(')
			parity++;
		else if (formula[pos] == ')')
			parity--;
		if (parity < 0)
			throw runtime_error("There is a right bracket without matching left bracket in the part \"" + formula + "\".");


		if (parity == 0 && ((formula[pos] == '|' && is_or) || (formula[pos] == '&' && !is_or)))
		{
			result.push_back(formula.substr(last_pos, pos - last_pos));
			last_pos = pos + 1;
		}
	}

	result.push_back(formula.substr(last_pos));

	if (parity > 0)
		throw runtime_error("There is a left bracket without matching right bracket in the part \"" + formula + "\".");

	return result;
}

void ConstraintParser::remove_parenthesis(string & formula)
{
	// If there's less than two characters there can't be no parenthesis.
	if (formula.size() < 2)
		return;
	// Parenthesis must be enclosing
	if (*formula.begin() != '(' || *formula.rbegin() != ')')
		return;
	// Only the last parenthesis must be matching
	size_t parity = 1;

	for (const size_t pos : crange(static_cast<size_t>(1), formula.size() - 1))
	{
		if (formula[pos] == '(')
			parity++;
		else if (formula[pos] == ')')
			parity--;
		if (parity == 0)
			return;
	}

	formula = formula.substr(1, formula.size() - 2);
}

Gecode::BoolExpr ConstraintParser::resolveFormula(const vector<string> & names, string formula)
{
	Gecode::BoolExpr result;

	// Remove outer parenthesis until you reach fixpoint.
	string old_formula;

	do
	{
		old_formula = formula;
		remove_parenthesis(formula);
	} while (old_formula.compare(formula) != 0);

	// Make divisions of the current expression by operators
	vector<string> div_by_or = splitByOperator(true, formula);
	vector<string> div_by_and = splitByOperator(false, formula);

	// Based on the divisions decide how to deal with the formula

	if (div_by_or.size() == 1 && div_by_and.size() == 1)
	{
		if (formula[0] == '!')
			return Gecode::BoolExpr(!resolveFormula(names, formula.substr(1)));
		else
			return convertAtom(names, formula);
	}

	else if (div_by_or.size() > 1 && div_by_and.size() == 1)
	{
		result = Gecode::BoolExpr(resolveFormula(names, div_by_or[0]) || resolveFormula(names, div_by_or[1]));
		for (const size_t expr_no : crange(static_cast<size_t>(2), div_by_or.size()))
			result = Gecode::BoolExpr(result || resolveFormula(names, div_by_or[expr_no]));
		return result;
	}

	else if (div_by_or.size() == 1 && div_by_and.size() > 1)
	{
		result = Gecode::BoolExpr(resolveFormula(names, div_by_and[0]) && resolveFormula(names, div_by_and[1]));
		for (const size_t expr_no : crange(static_cast<size_t>(2), div_by_and.size()))
			result = Gecode::BoolExpr(result && resolveFormula(names, div_by_and[expr_no]));
		return result;
	}

	else
	{
		throw runtime_error("Error when parsing the part \"" + formula + "\" Operators | and & are mixed, add parenthesis.");
	}
}

ConstraintParser::ConstraintParser(const size_t number, const size_t upper_bound) : allowed_vals(*this, number, 0, upper_bound)
{
	if (number == 0)
		throw invalid_argument("A call for constraint parser with no variables.");
	branch(*this, allowed_vals, Gecode::INT_VAR_SIZE_MIN(), Gecode::INT_VAL_MIN());
}

ConstraintParser::ConstraintParser(bool share, ConstraintParser &other_space) : Space(share, other_space)
{
	allowed_vals.update(*this, share, other_space.allowed_vals);
}

Gecode::Space* ConstraintParser::copy(bool share)
{
	return new ConstraintParser(share, *this);
}

void ConstraintParser::addBoundaries(const Levels & boundaries, const bool is_upper)
{
	if (boundaries.size() != allowed_vals.size())
		throw runtime_error("Trying to bound the space of solutions in a constraint parser "
			"but the number of boundaries does not match the number of variables.");
	auto op = is_upper ? Gecode::IRT_LQ : Gecode::IRT_GQ;
	for (const ActLevel i : cscope(boundaries))
		rel(*this, allowed_vals[i], op, (boundaries[i]));
}

void ConstraintParser::applyFormula(const vector<string> & names, string formula)
{
	// Remove spaces
	formula.erase(remove_if(formula.begin(), formula.end(), (int(*)(int))isspace), formula.end());
	Gecode::BoolExpr expr = resolveFormula(names, formula.empty() ? "tt" : formula);
	rel(*this, expr);
}

void ConstraintParser::print(void) const
{
	std::cout << allowed_vals << std::endl;
}

Levels ConstraintParser::getBounds(bool upper) const
{
	Levels result(allowed_vals.size(), 0u);

	for (const size_t i : cscope(allowed_vals))
		result[i] = upper ? allowed_vals[i].max() : allowed_vals[i].min();

	return result;
}

Levels ConstraintParser::getSolution() const
{
	Levels result(allowed_vals.size(), 0u);

	for (const size_t i : cscope(allowed_vals))
		result[i] = allowed_vals[i].val();

	return result;
}

bool ConstraintParser::contains(const vector<string> & names, const ActLevel max, const Levels & required, const string & formula)
{
	Levels maxes(names.size(), max);
	return contains(names, maxes, required, formula);
}

bool ConstraintParser::contains(const vector<string> & names, const Levels & maxes, const Levels & required, const string & formula)
{
	ConstraintParser *constraint_parser = new ConstraintParser(maxes.size(), *max_element(maxes.begin(), maxes.end()));
	// Force the values by bounding from the both sides and add the formula.
	constraint_parser->addBoundaries(maxes, true);
	constraint_parser->addBoundaries(required, true);
	constraint_parser->addBoundaries(required, false);
	constraint_parser->applyFormula(names, formula);

	// Return true iff a solution is found
	Gecode::DFS<ConstraintParser> search(constraint_parser);

	ConstraintParser * new_par = search.next();
	delete constraint_parser;
	return new_par != 0;
}