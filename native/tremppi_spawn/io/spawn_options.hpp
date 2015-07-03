#pragma once

#include <tremppi_common/header.h>
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/general/program_options.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief User options interpretation. Parses the options, terminates on --version and --help.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpawnOptions : public ProgramOptions 
{
public:
	/* Parse the program options - if help or version is required, terminate the program immediatelly. */

	bpo::variables_map parseProgramOptions(int argc, char ** argv) 
	{
		visible.add_options()
			("check-only,c", "do not create a database and only verify the file syntax");

		return ProgramOptions::parseProgramOptions(argc, argv);
	}
};

