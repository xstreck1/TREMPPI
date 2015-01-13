#pragma once

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/program_options.hpp>

class PythonOptions  {
public:
	bpo::variables_map parseProgramOptions(int argc, char ** argv) {
		bpo::variables_map result;
		result.insert(make_pair( "path", bpo::variable_value(bfs::current_path().string(), false)));
		return result;
	}
};