#pragma once

#include <tpi_common/general/common_functions.hpp>
#include <tpi_common/network/definitions.hpp>

#include <gecode/kernel.hh>
#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/minimodel.hh>

using namespace Gecode;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief A class that accepts and parser constraints from a string formula and provides solutions to these constraints.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ConstraintParser : public Space {
	IntVarArray allowed_vals; ///< The actual values

	/* Transform the string into an integer, if possible. Return true iff sucessful. */
	bool getNumber(string atom_part, int & value) {
		try {
			value = lexical_cast<int>(atom_part);
		}
		catch (...) {
			return false;
		}
		return true;
	}

	/* Find the number of the integer variable that corresponds to the given name of the specie. */
	size_t findName(const vector<string> & names, string specie_name) {
		for (const size_t name_no : cscope(names))
		if (specie_name.compare(names[name_no]) == 0)
			return name_no;
		throw runtime_error("Unrecognized variable name \"" + specie_name + "\".");
	}

	/* Apply the respective operator on the operands that are either converted to constants or matched to variables by name. */
	LinIntRel applyOperator(const vector<string> & names, const string left_side, const string right_side, Gecode::IntRelType oper) {
		int left_val = 0, right_val = 0;
		if (getNumber(left_side, left_val))
			return LinIntRel(left_val, oper, allowed_vals[findName(names, right_side)]);
		else if (getNumber(right_side, right_val))
			return LinIntRel(allowed_vals[findName(names, left_side)], oper, right_val);
		else
			return LinIntRel(allowed_vals[findName(names, left_side)], oper, allowed_vals[findName(names, right_side)]);
	}

	/* Convert the atomic expression to the relevant constraint */
	BoolExpr convertAtom(const vector<string> & names, const string & atom) {
		if (atom.compare("tt") == 0)
			return BoolExpr(allowed_vals[0] == allowed_vals[0]);
		else if (atom.compare("ff") == 0)
			return BoolExpr(allowed_vals[0] != allowed_vals[0]);
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
			return LinIntRel(allowed_vals[findName(names, atom)] == 1);
	}

	/* Split the formula by the specified operator (either | or &). This is an intelligent split - only symbols that are outside parenthesis are considered. */
	vector<string> splitByOperator(const bool is_or, const string & formula) {
		vector<string> result;

		int parity = 0;
		size_t last_pos = 0;
		for (const size_t pos : cscope(formula)) {
			if (formula[pos] == '(')
				parity++;
			else if (formula[pos] == ')')
				parity--;
			if (parity < 0)
				throw runtime_error("There is a right bracket without matching left bracket in the part \"" + formula + "\".");

			if (parity == 0 && ((formula[pos] == '|' && is_or) || (formula[pos] == '&' && !is_or))) {
				result.push_back(formula.substr(last_pos, pos - last_pos));
				last_pos = pos + 1;
			}
		}

		result.push_back(formula.substr(last_pos));

		if (parity > 0)
			throw runtime_error("There is a left bracket without matching right bracket in the part \"" + formula + "\".");

		return result;
	}

	/* Trim the current formula and if it is enclosed in parenthesis, remove them. */
	void remove_parenthesis(string & formula) {
		// If there's less than two characters there can't be no parenthesis.
		if (formula.size() < 2)
			return;
		// Parenthesis must be enclosing
		if (*formula.begin() != '(' || *formula.rbegin() != ')')
			return;
		// Only the last parenthesis must be matching
		size_t parity = 1;
		for (const size_t pos : crange(static_cast<size_t>(1), formula.size() - 1)) {
			if (formula[pos] == '(')
				parity++;
			else if (formula[pos] == ')')
				parity--;
			if (parity == 0)
				return;
		}

		formula = formula.substr(1, formula.size() - 2);
	}

	/*  */
	BoolExpr resolveFormula(const vector<string> & names, string formula) {
		BoolExpr result;

		// Remove outer parenthesis until you reach fixpoint.
		string old_formula;
		do {
			old_formula = formula;
			remove_parenthesis(formula);
		} while (old_formula.compare(formula) != 0);

		// Make divisions of the current expression by operators
		vector<string> div_by_or = splitByOperator(true, formula);
		vector<string> div_by_and = splitByOperator(false, formula);

		// Based on the divisions decide how to deal with the formula
		if (div_by_or.size() == 1 && div_by_and.size() == 1) {
			if (formula[0] == '!')
				return BoolExpr(!resolveFormula(names, formula.substr(1)));
			else
				return convertAtom(names, formula);
		}
		else if (div_by_or.size() > 1 && div_by_and.size() == 1) {
			result = BoolExpr(resolveFormula(names, div_by_or[0]) || resolveFormula(names, div_by_or[1]));
			for (const size_t expr_no : crange(static_cast<size_t>(2), div_by_or.size()))
				result = BoolExpr(result || resolveFormula(names, div_by_or[expr_no]));
			return result;
		}
		else if (div_by_or.size() == 1 && div_by_and.size() > 1) {
			result = BoolExpr(resolveFormula(names, div_by_and[0]) && resolveFormula(names, div_by_and[1]));
			for (const size_t expr_no : crange(static_cast<size_t>(2), div_by_and.size()))
				result = BoolExpr(result && resolveFormula(names, div_by_and[expr_no]));
			return result;
		}
		else {
			throw runtime_error("Error when parsing the part \"" + formula + "\" Operators | and & are mixed, add parenthesis.");
		}
	}

public:
	// For safety, do not allow any other sort of creating the parser than by dedicated functions
	ConstraintParser() = delete;
	ConstraintParser(ConstraintParser &&) = delete;
	ConstraintParser& operator=(ConstraintParser &&) = delete;
	ConstraintParser(const ConstraintParser &) = delete;
	ConstraintParser& operator=(const ConstraintParser &) = delete;

	ConstraintParser(const size_t number, const size_t upper_bound)
		: allowed_vals(*this, number, 0, upper_bound) {
		if (number == 0)
			throw invalid_argument("A call for constraint parser with no variables.");
		branch(*this, allowed_vals, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
	}

	ConstraintParser(bool share, ConstraintParser &other_space)
		: Space(share, other_space) {
		allowed_vals.update(*this, share, other_space.allowed_vals);
	}

	virtual Space *copy(bool share) {
		return new ConstraintParser(share, *this);
	}

	void addBoundaries(const Levels & boundaries, const bool is_upper) {
		if (boundaries.size() != allowed_vals.size())
			throw runtime_error("Trying to bound the space of solutions in a constraint parser "
			"but the number of boundaries does not match the number of variables.");
		auto op = is_upper ? IRT_LQ : IRT_GQ;
		for (const ActLevel i : cscope(boundaries))
			rel(*this, allowed_vals[i], op, (boundaries[i]));
	}

	/* Take a logical formula and make it into a constraint that gets propagated. */
	void applyFormula(const vector<string> & names, string formula) {
		// Remove spaces
		formula.erase(remove_if(formula.begin(), formula.end(), (int(*)(int))isspace), formula.end());
		BoolExpr expr = resolveFormula(names, formula);
		rel(*this, expr);
	}

	// print solution
	void print(void) const {
		std::cout << allowed_vals << std::endl;
	}

	// get boundaries after propagation
	Levels getBounds(bool upper) const {
		Levels result(allowed_vals.size(), 0u);

		for (const size_t i : cscope(allowed_vals))
			result[i] = upper ? allowed_vals[i].max() : allowed_vals[i].min();

		return result;
	}

	/* Obtain a vector with the solution */
	Levels getSolution() const {
		Levels result(allowed_vals.size(), 0u);

		for (const size_t i : cscope(allowed_vals))
			result[i] = allowed_vals[i].val();

		return result;
	}

	// True iff the required result is in between solution of the formula
	static bool contains(const vector<string> & names, const ActLevel max, const Levels & required, const string & formula) {
		Levels maxes(names.size(), max);
		return contains(names, maxes, required, formula);
	}

	// True iff the required result is in between solution of the formula
	static bool contains(const vector<string> & names, const Levels & maxes, const Levels & required, const string & formula) {
		ConstraintParser *constraint_parser = new ConstraintParser(maxes.size(), *max_element(maxes.begin(), maxes.end()));
		// Force the values by bounding from the both sides and add the formula.
		constraint_parser->addBoundaries(maxes, true);
		constraint_parser->addBoundaries(required, true);
		constraint_parser->addBoundaries(required, false);
		constraint_parser->applyFormula(names, formula);

		// Return true iff a solution is found
		DFS<ConstraintParser> search(constraint_parser);

		ConstraintParser * new_par = search.next();
		delete constraint_parser;
		return new_par != 0;
	}
};