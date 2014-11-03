#pragma once

#include <tremppi_common/header.h>
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/general/program_options.hpp>

class ReportOptions : public ProgramOptions {
public:
	/* Parse the program options - if help or version is required, terminate the program immediatelly. */
	bpo::variables_map parseProgramOptions(int argc, char ** argv) {
		visible.add_options()
			("all,a", "do not consider the select or compare clause and apply to all parametrizations without comparison")
			("select-only,s", "do not consider the compare clause and just analyze the selection")
			("graph,g", "build the parametrization space graph")
			("functions,f", "compute function analysis and outputs correlation graph")
			("regulations,r", "outputs the overlapping regulatory graph")
			;

		return ProgramOptions::parseProgramOptions(argc, argv);
	}
};


