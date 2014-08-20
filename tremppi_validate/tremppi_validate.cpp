#include <tremppi_common/general/logging.hpp>

#include "io/program_options.hpp"

using namespace TremppiValidate;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_validate.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_validate(int argc, char ** argv) {
	bpo::variables_map po; // program options provided on the command line
	bfs::path input_path; // an input path

	try {
		if (argc < 1)
			throw runtime_error("No parameters.");

		po = ProgramOptions::parseProgramOptions(argc, argv);
		input_path = ProgramOptions::getDatabasePath(po);

		tremppi_system.set("tremppi_validate", argv[0], input_path.parent_path());
		logging.init(1);
		BOOST_LOG_TRIVIAL(info) << "TREMPPI efficient paremetrization validation by LTL properties (" << tremppi_system.PROGRAM_NAME << ") started.";
	}
	catch (exception & e) {
		cerr << e.what();
		return 1;
	}

	BOOST_LOG_TRIVIAL(info) << tremppi_system.PROGRAM_NAME << " finished successfully.";
	return 0;
}
