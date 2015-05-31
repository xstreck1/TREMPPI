#pragma once

#include <tremppi_common/header.h>
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/general/program_options.hpp>

class regulationsOptions : public ProgramOptions {
public:
	/* Parse the program options - if help or version is required, terminate the program immediatelly. */
	bpo::variables_map parseProgramOptions(int argc, char ** argv) {
		return ProgramOptions::parseProgramOptions(argc, argv);
	}
};


