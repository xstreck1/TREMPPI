#include <tpi_common/general/logging.hpp>
#include "io/constraint_parser.hpp"
#include "io/program_options.hpp"
#include "io/syntax_checker.hpp"

// TODO: disable regulatory if not -r
int main(int argc, char ** argv) {
	Logging::init(PROGRAM_NAME + ".log");
	BOOST_LOG_TRIVIAL(info) << "TREMPPI Parametrization database builder (tpi_stat) started.";

	bpo::variables_map po; // program options provided on the command line
	bfs::path input_path; // an input pah
	Json::Value root; // root of the network

	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing network file.";

		po = ProgramOptions::parseProgramOptions(argc, argv);
		input_path = ProgramOptions::getNetworkPath(po);

		ifstream network_file(input_path.string(), ios::in);
		if (!network_file)
			throw runtime_error("Could not open the file " + input_path.string() + " for reading.");
		root = SyntaxChecker::readFile(network_file);

		SyntaxChecker::controlSemantics(root["elements"]);
	}
	catch (exception & e) {
		Logging::exceptionMessage(e, 1);
	}

	BOOST_LOG_TRIVIAL(info) << PROGRAM_NAME << " finished successfully.";
	return 0;
}
