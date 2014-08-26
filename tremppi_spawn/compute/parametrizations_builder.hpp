#pragma once

#include <tremppi_common/network/constraint_parser.hpp>
#include <tremppi_common/network/kinetics.hpp>
#include "../io/constraint_formatter.hpp"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Class that computes feasible parametrizations for each specie from
/// the edge constrains and stores them in a ParametrizationHolder object.
///
/// This construction may be optimized by including the warm-start constraint satisfaction.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ParametrizationsBuilder {
	/**
	* @brief isSubordinate returns true if the current context is the same as the compared context only with a higher activity value in specificed regulator.
	*/
	static bool isSubordinate(const vector<Model::Regulation> &reguls, const Kinetics::Param &current, const Kinetics::Param &compare, const CompID source_ID) {
		for (const Model::Regulation & regul : reguls) {
			const CompID regul_ID = regul.source.id;
			// All the regulations must have the same requirements, except for the one with the specified source, which must connect on the value.
			if (regul_ID != source_ID) {
				if (current.requirements.find(regul_ID)->second.back() != compare.requirements.find(regul_ID)->second.back()) {
					return false;
				}
			}
			else if (current.requirements.find(regul_ID)->second.front() != compare.requirements.find(regul_ID)->second.back() + 1) {
				return false;
			}

		}
		return true;
	}

	/**
	* Return true if the given parameter's context is dependent on the given regulation.
	*/
	static bool containsRegulation(const Kinetics::Param &param_data, const Model::Regulation &regul) {
		return param_data.requirements.find(regul.source.id)->second.front() == regul.threshold;
	}

	/* initial constraining of the values to the predefined ones */
	static string addAllowed(const Levels &targets, const string & context) {
		string result = "ff ";

		// Add only present values
		for (const size_t val : targets)
			result += " | " + to_string(val) + " = " + context;

		return result;
	}

	/* Create an expression based on the conditions obtained from the edge label */
	static string replaceInLabel(const string & label, const string & plus, const string & minus) {
		string result = label;

		auto replace = [&result](const char symbol, const string & formula) -> void {
			size_t pos = result.find(symbol);
			while (pos != result.npos) {
				result.replace(pos, 1, formula);
				pos = result.find(symbol);
			}
		};

		replace('+', plus);
		replace('-', minus);

		return result;
	}

	/* For each regulation create a constraint corresponding to its label */
	static void createEdgeCons(const vector<Model::Regulation> & reguls, const Kinetics::Params & params, const Model::Regulation & regul, string & plus, string & minus) {
		plus = minus = "ff ";
		for (const auto param : params) {
			if (containsRegulation(param, regul)) {
				for (const auto compare : params) {
					if (isSubordinate(reguls, param, compare, regul.source.id)) {
						plus += " | " + param.context + " > " + compare.context;
						minus += " | " + param.context + " < " + compare.context;
					}
				}
			}
		}
	}

	static void addBrackets(string & formula) {
		formula = "(" + formula + ")";
	}

	static string createFormula(const vector<Model::Regulation> & reguls, const Kinetics::Params & params) {
		string result = "tt ";

		// Add constraints for all the regulations
		for (auto & regul : reguls) {
			string plus, minus, label;
			createEdgeCons(reguls, params, regul, plus, minus);
			addBrackets(plus);
			addBrackets(minus);
			label = ModelTranslators::readLabel(regul.label);
			label = replaceInLabel(label, plus, minus);
			addBrackets(label);
			result += " & " + label;
		}

		// List all the possible target values for a parameter
		for (auto & param : params) {
			string allowed;
			allowed = addAllowed(param.targets, param.context);
			addBrackets(allowed);
			result += " & " + allowed;
		}

		return result;
	}

	/* Create constraint space on parametrizations for the given specie and enumerate and store all the solutions. */
	static Configurations  createPartCol(const Kinetics::Params & params, const string formula, const size_t max_value) {
		Configurations result;

		// Build the space
		vector<string> names;
		for (const auto & param : params)
			names.push_back(param.context);

		ConstraintParser * cons_pars = new ConstraintParser(names.size(), max_value);

		// Impose constraints
		cons_pars->applyFormula(names, formula);

		// Conduct search
		Gecode::DFS<ConstraintParser> search(cons_pars);
		delete cons_pars;
		while (ConstraintParser *match = search.next()) {
			Levels solution = match->getSolution();
			Levels shortened;
			result.push_back(solution);
			delete match;
		}

		return result;
	}

public:
	/**
	* Entry function of parsing, tests and stores subcolors for all the components.
	*/
	static void build(const Model &model, Kinetics & kinetics) {
		ParamNo step_size = 1; // Variable necessary for encoding of colors

		// Cycle through components
		for (CompID ind = model.components.size(); ind > 0; --ind) {

			CompID ID = ind - 1;
			kinetics.components[ID].step_size = step_size;
			kinetics.components[ID].col_count = 1;

			if (kinetics.components[ID].params.empty())
				continue;

			// Solve the parametrizations
			string formula = createFormula(model.components[ID].regulations, kinetics.components[ID].params) + " & " + ConstraintFomatter::consToFormula(model, ID);
			Configurations subcolors = createPartCol(kinetics.components[ID].params, formula, model.components[ID].max_activity);

			// Copy the data
			auto & params = kinetics.components[ID].params;
			for (const Levels & subcolor : subcolors)
				for (const size_t param_no : cscope(subcolor))
					params[param_no].target_in_subcolor.emplace_back(subcolor[param_no]);


			kinetics.components[ID].col_count = subcolors.size();
			step_size *= subcolors.size();
		}
	}
};