#pragma once

#include <tremppi_common/network/kinetics_translators.hpp>
#include <tremppi_common/network/model_translators.hpp>

#include "synthesis_manager.hpp"
#include "witness_searcher.hpp"
#include "robustness_compute.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Class that outputs formatted resulting data.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OutputManager {
	const bpo::variables_map & user_options; ///< User can influence the format of the output.
	const PropertyAutomaton & property; ///< Property automaton.
	const Model & model; ///< Reference to the model itself.
	const Kinetics & kinetics; ///< Kinetics data for the model

public:
	NO_COPY(OutputManager)

	OutputManager(const bpo::variables_map  & _user_options, const PropertyAutomaton & _property, const Model & _model, const Kinetics & _kinetics)
		: user_options(_user_options), property(_property), model(_model), kinetics(_kinetics) {}

public:
	/**
	 * @brief outputForm
	 */
	void outputForm() {
		string format_desc = "#:(";

		for (CompID ID : crange(model.components.size())) {
			for (const auto & param : kinetics.components[ID].params) {
				format_desc += model.components[ID].name + "{" + param.context + "},";
			}
		}
		format_desc.back() = ')';
		format_desc += ":Cost:Robust:Witness";
	}

	/**
	 * Output parametrizations from this round together with additional data, if requested.
	 */
	void outputRound(const size_t param_ID, const size_t & cost, const double robustness_val, const string & witness) {
		string param_vals = KineticsTranslators::createParamString(kinetics, param_ID);
		string line = to_string(param_ID) + separator + param_vals + separator;
		string update = param_vals.insert(1, to_string(param_ID) + ",");
		update.back() = ','; // must remove closing bracket, it will be added by database manager

		if (cost != INF)
			line += to_string(cost);
		line += separator;
		update += to_string(cost) + ",";

		string robustness = robustness_val > 0. ? to_string(robustness_val) : "\"\"";
		line += robustness + separator;
		update += robustness + ",";

		line += witness + separator;
		update += "\"" + witness + "\")";

		size_t traits = 0;
	}
};