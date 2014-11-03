#pragma once

#include <tremppi_common/header.h>
#include <tremppi_common/network/definitions.hpp>
#include <tremppi_common/general/program_options.hpp>

class ExpressOptions : public ProgramOptions {
public:
	/* Parse the program options - if help or version is required, terminate the program immediatelly. */
	bpo::variables_map parseProgramOptions(int argc, char ** argv) {
		visible.add_options()
			("all,a", "do not consider the select clause and apply to all parametrizations")
			;

		return ProgramOptions::parseProgramOptions(argc, argv);
	}
};

