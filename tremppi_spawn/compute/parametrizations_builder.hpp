/*
* Copyright (C) 2012-2013 - Adam Streck
* This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
* ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
* ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
* For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
*/

#pragma once

#include <tremppi_common/data_info.hpp>
#include <tremppi_common/common_functions.hpp>
#include <tremppi_common/data_types.hpp>

#include "../io/constraint_parser.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Class that computes feasible parametrizations for each specie from
/// the edge constrains and stores them in a ParametrizationHolder object.
///
/// This construction may be optimized by including the warm-start constraint
/// satisfaction.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ParametrizationsBuilder {
	/**
	* @brief isSubordinate returns true if the current context is the same as the compared context only with a higher activity value in specificed regulator.
	*/
	bool isSubordinate(const vector<Model::Regulation> &reguls, const Kinetics::Param &current, const Kinetics::Param &compare, const SpecieID source_ID) {
		for (const Model::Regulation & regul : reguls) {
			const SpecieID regul_ID = regul.source;
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
	bool containsRegulation(const Kinetics::Param &param_data, const Model::Regulation &regul) {
		return param_data.requirements.find(regul.source)->second.front() == regul.threshold;
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
					if (isSubordinate(reguls, param, compare, regul.source)) {
						plus += " | " + param.context + " > " + compare.context;
						minus += " | " + param.context + " < " + compare.context;
					}
				}
			}
		}
	}

	static void addParenthesis(string & formula) {
		formula = "(" + formula + ")";
	}

	static string createFormula(const vector<Model::Regulation> & reguls, const Kinetics::Params & params) {
		string result = "tt ";

		// Add constraints for all the regulations
		for (auto & regul : reguls) {
			string plus, minus, label;
			createEdgeCons(reguls, params, regul, plus, minus);
			addParenthesis(plus);
			addParenthesis(minus);
			label = ModelHelper::readLabel(regul.label);
			label = replaceInLabel(label, plus, minus);
			addParenthesis(label);
			result += " & " + label;
		}

		// List all the possible target values for a parameter
		for (auto & param : params) {
			string allowed;
			allowed = addAllowed(param.targets, param.context);
			addParenthesis(allowed);
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
		DFS<ConstraintParser> search(cons_pars);
		delete cons_pars;
		while (ConstraintParser *match = search.next()) {
			Levels solution = match->getSolution();
			Levels shortened;
			result.push_back(solution);
			delete match;
		}

		return result;
	}

	static void add_irrelevant(Kinetics::Params & params, Configurations & subcolors) {
		for_each(WHOLE(subcolors), [&params](Levels & subcolor){
			for (const size_t param_no : cscope(params))
				if (!params[param_no].functional)
					subcolor[param_no] = -1;
		});
	}

	static void remove_redundant(Kinetics::Params & params, Configurations & subcolors) {
		auto new_end = unique(begin(subcolors), end(subcolors));
		subcolors.resize(distance(begin(subcolors), new_end));
	}

public:
	/**
	* Entry function of parsing, tests and stores subcolors for all the species.
	*/
	static void buildParametrizations(const Model &model, Kinetics & kinetics) {
		ParamNo step_size = 1; // Variable necessary for encoding of colors

		// Cycle through species
		for (SpecieID ind = model.species.size(); ind > 0; --ind) {
			output_streamer.output(verbose_str, "Testing edge constraints for Specie: " + to_string(ind) + "/"
				+ to_string(model.species.size()) + ".", OutputStreamer::no_newl | OutputStreamer::rewrite_ln);

			SpecieID ID = ind - 1;
			kinetics.species[ID].step_size = step_size;

			if (model.species[ID].spec_type == Model::Input) {
				kinetics.species[ID].col_count = 1;
			}
			else {

				// Solve the parametrizations
				string formula = createFormula(model.species[ID].regulations, kinetics.species[ID].params) + " & " + ConstraintReader::consToFormula(model, ID);
				Configurations subcolors = createPartCol(kinetics.species[ID].params, formula, model.species[ID].max_value);
				// add_irrelevant(kinetics.species[ID].params, subcolors);
				// sort(WHOLE(subcolors));
				// remove_redundant(kinetics.species[ID].params, subcolors);

				// Copy the data
				auto & params = kinetics.species[ID].params;
				for (const Levels & subcolor : subcolors)
					for (const size_t param_no : cscope(subcolor))
						// if (params[param_no].functional)
						params[param_no].target_in_subcolor.emplace_back(subcolor[param_no]);


				kinetics.species[ID].col_count = subcolors.size();
				step_size *= subcolors.size();
			}
		}

		output_streamer.clear_line(verbose_str);
		output_streamer.output(verbose_str, "", OutputStreamer::no_out | OutputStreamer::rewrite_ln | OutputStreamer::no_newl);
	}
};