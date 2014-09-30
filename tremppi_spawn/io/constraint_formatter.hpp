#pragma once

#include "../data/model_translators.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Parses contrints as specified in the model and return a formula built from these constraints.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ConstraintFomatter {
	/* Context may contain specie-characters or number or colon or comma*/
	static bool belongsToContext(const char ch) {
		return isalpha(ch) || isdigit(ch) != 0 || ch == ':' || ch == ',' || ch == '_';
	}

	static string addBrackets(string expr) {
		return string("(" + expr + ")");
	}

	/* Take the original constraint and replace contexts specified with their canonic version. */
	static string formatConstraint(string original, const Model & model, const CompID ID) {
		string result;

		// Copy all the characters, replace contexts for canonic versions
		size_t start = INF;
		for (const size_t pos : cscope(original)) {
			if (start == INF) {
				if (isalpha(original[pos])) {
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

	// Reformulate explicit user constraint so it matches the format of the constraint parser
	static string consToFormula(const Model & model, const CompID ID) {
		string formula; // Resulting formula specifying all the constraints
		
		formula = "tt";

		for (const string constraint : model.components[ID].constraints) {
			try {
				formula.append(" & " + addBrackets(formatConstraint(constraint, model, ID)));
			}
			catch (exception & e) {
				throw runtime_error("Error while parsing the constraint: " + quote(constraint) + ". " + e.what());
			}
		}
		
		return addBrackets(formula);
	}
};