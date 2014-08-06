#include <tpi_common/database_reader.hpp>
#include <tpi_common/file_manipulation.hpp>
#include <tpi_common/logging.hpp>
#include <tpi_common/sqlite3pp_func.hpp>
#include <tpi_common/time_manager.hpp>
#include "io/program_options.hpp"
#include "io/model_reader.hpp"

// TODO: disable regulatory if not -r
int main(int argc, char ** argv) {
	Logging::init(PROGRAM_NAME + ".log");
	BOOST_LOG_TRIVIAL(info) << "TREMPPI Parametrization database builder (tpi_stat) started.";

	bpo::variables_map po;
	bfs::path input_path;
	Json::Value root;   // root of the network

	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing network file.";

		po = ProgramOptions::parseProgramOptions(argc, argv);
		input_path = ProgramOptions::getNetworkPath(po);

		ifstream network_file(input_path.string(), ios::in);
		if (!network_file)
			throw runtime_error("Could not open the file " + input_path.string() + " for reading.");
		root = ModelReader::readFile(network_file);

		ModelReader::controlSemantics(root["elements"]);
	}
	catch (exception & e) {
		Logging::exceptionMessage(e, 1);
	}

	BOOST_LOG_TRIVIAL(info) << PROGRAM_NAME << " finished succesfully.";
	return 0;
}
