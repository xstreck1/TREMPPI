#pragma once

#include <tremppi_common/header.h>
#include <tremppi_common/network/definitions.hpp>
#include <tremppi_common/general/program_options.hpp>

enum TraceType { none, rob, wit }; //< What sort of trace the user requests - no trace, computation of robustness only or the transitions of the trace
enum BoundType { inf, min, step }; //< What sort of bound the user placed - do the full check / check until the minimal lenght is found for a parametrization / use only bounded amount of steps

class ValidateOptions : public ProgramOptions {
public:
	/* Parse the program options - if help or version is required, terminate the program immediatelly. */
	bpo::variables_map parseProgramOptions(int argc, char ** argv) {
		visible.add_options()
			("select", bpo::value<string>(), "selection of the set of interest")
			("trace", bpo::value<string>()->default_value("none"), "what form of trace analysis should be applied, one of {none, rob, wit}")
			("bound", bpo::value<string>()->default_value("inf"), "what bound is placed on the search, one of {inf, min, N} where N is a non-zero positive integer")
			;

		return ProgramOptions::parseProgramOptions(argc, argv);
	}

	// @obtains the exact numeric bound from the parsed options. If the bound can't be obtained, return INF
	static size_t getBound(const bpo::variables_map & po) {
		if (regex_match(po["bound"].as<string>(), regex("\\d+")))
			return po["bound"].as<size_t>();
		else
			return INF;
	}

	// @brief:	obtains the trace type from the parsed options, also does syntax check
	static TraceType getTracteType(const bpo::variables_map & po)  {
		string input = po["trace"].as<string>();
		if (input == "none")
			return TraceType::none;
		else if (input == "rob")
			return TraceType::rob;
		else if (input == "wit")
			return TraceType::wit;
		else
			throw runtime_error("Unknown trace type specified: " + input);
	}

	// @brief:	obtains the bound type from the parsed options, also does syntax check
	static BoundType getBoundType(const bpo::variables_map & po) {
		string input = po["bound"].as<string>();
		if (input == "inf")
			return BoundType::inf;
		else if (input == "min")
			return BoundType::min;
		else if (regex_match(input, regex("\\d+")))
			return BoundType::step;
		else
			throw runtime_error("Unknown bound type specified: " + input);
	}
};
