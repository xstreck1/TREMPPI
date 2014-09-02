#pragma once

#include <tremppi_common/network/data_info.hpp>

#include "synthesis_manager.hpp"
#include "witness_searcher.hpp"
#include "robustness_compute.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Class that outputs formatted resulting data.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OutputManager {
	const bpo::variables_map & user_options; ///< User can influence the format of the output.
	const PropertyAutomaton & property; ///< Property automaton.
	const RegInfos & reg_infos; ///< Reference to the model itself.

public:
	NO_COPY(OutputManager)

	OutputManager(const bpo::variables_map  & _user_options, const PropertyAutomaton & _property, const RegInfos & _reg_infos)
	: user_options(_user_options), property(_property), reg_infos(_reg_infos) {}

public:
	/**
	 * @brief outputForm
	 */
	void outputForm() {
		string format_desc = "#:(";

		for (CompID ID : crange(reg_infos.size())) {
			for (const auto & column : reg_infos[ID].columns) {
				format_desc += reg_infos[ID].name + "{" + column.second + "},";
			}
		}
		format_desc.back() = ')';
		format_desc += ":Cost:Robust:Witness";
	}

	/**
	 * Output parametrizations from this round together with additional data, if requested.
	 */
	void outputRound(const size_t & cost, const double robustness_val, const string & witness, const Levels & parametrization, const ParamNo & rowid) {
		string line;
		for (const ActLevel param_val : parametrization)
			line += to_string(param_val) + ",";
		line.back() = separator;

		string update = line;
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