#pragma once

#include <tremppi_common/header.h>
#include <tremppi_common/network/definitions.hpp>
#include <tremppi_common/general/program_options.hpp>

class ExpressOptions : public ProgramOptions {
public:
	/* Parse the program options - if help or version is required, terminate the program immediatelly. */
	bpo::variables_map parseProgramOptions(int argc, char ** argv) {
		visible.add_options()
			("select", bpo::value<string>(), "selection of the set of interest")
			;

		return ProgramOptions::parseProgramOptions(argc, argv);
	}
};

