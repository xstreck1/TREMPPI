#pragma once

#include <tremppi_common/header.h>
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/general/program_options.hpp>

class ReportOptions : public ProgramOptions {
public:
	/* Parse the program options - if help or version is required, terminate the program immediatelly. */
	bpo::variables_map parseProgramOptions(int argc, char ** argv) {
		visible.add_options()
			("select", bpo::value<string>(), "selection of the set of interest")
			("graph,g", "build the parametrization space graph")
			("functions,f", "compute function analysis and outputs correlation graph")
			("regulations,r", "outputs the overlapping regulatory graph")
			("select", bpo::value<string>(), "selection of the set of interest")
			("compare", bpo::value<string>(), "selection of the set the main set should be compared with")
			;

		return ProgramOptions::parseProgramOptions(argc, argv);
	}
};


