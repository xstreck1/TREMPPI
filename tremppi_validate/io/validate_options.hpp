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
		return ProgramOptions::parseProgramOptions(argc, argv);
	}
};
