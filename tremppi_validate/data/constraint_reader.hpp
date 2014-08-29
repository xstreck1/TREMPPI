#pragma once

#include <tremppi_common/network/model_translators.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Parses contrints as specified in the model and return a formula built from these constraints.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ConstraintReader {
	/* Specie can start with a letter or with _ */
	static bool initiatesContext(const char ch) {
		return isalpha(ch) != 0 || ch == '_';
	}

	/* Context may contain specie-characters or number or colon or comma*/
	static bool belongsToContext(const char ch) {
		return initiatesContext(ch) || isdigit(ch) != 0 || ch == ':' || ch == ',';
	}

	static string addParenthesis(string expr) {
		return string("(" + expr + ")");
	}

	/* Take the original constraint and replace contexts specified with their canonic version. */
	static string formatConstraint(string original, const Model & model, const CompID ID) {
		string result;

		// Copy all the characters, replace contexts for canonic versions
		size_t start = INF;
		for (const size_t pos : cscope(original)) {
			if (start == INF) {
				if (initiatesContext(original[pos])) {
					start = pos;
				}
				else {
					result.push_back(original[pos]);
				}
			}
			else {
				if (!belongsToContext(original[pos])) {
					string context = original.substr(start, pos - start);
					result.append(ModelTranslators::makeCanonic(model, context, ID));

					result.push_back(original[pos]);
					start = INF;
				}
			}
		}

		// Add the last context in the canonic form
		if (start != INF) {
			string context = original.substr(start);
			result.append(ModelTranslators::makeCanonic(model, context, ID));
		}

		return result;
	}
public:

	static string consToFormula(const Model & model, const CompID ID) {
		string formula; // Resulting formula specifying all the constraints
		
		formula = "tt";

		for (const string constraint : model.components[ID].constraints) {
			formula.append(" & " + addParenthesis(formatConstraint(constraint, model, ID)));
		}
		
		return addParenthesis(formula);
	}
};
