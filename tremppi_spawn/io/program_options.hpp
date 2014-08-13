#pragma once

#include <tremppi_common/header.h>
#include <tremppi_common/general/common_functions.hpp>

const string PROGRAM_NAME = "tremppi_spawn";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief User options interpretation. Parses the options, terminates on --version and --help.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace ProgramOptions {
	/* Parse the program options - if help or version is required, terminate the program immediatelly. */
	bpo::variables_map parseProgramOptions(int argc, char ** argv) {
		bpo::variables_map result;

		// Declare the supbported options.
		bpo::options_description visible("Execution options");
		visible.add_options()
			("help,h", "display help")
			("version,v", "display version")
			("check-only,c", "do not create a database and only verify the file syntax")
			;
		bpo::options_description invisible;
		invisible.add_options()
			("network", bpo::value<string>()->required(), ("Database file with " + NETWORK_SUFFIX + " suffix").c_str())
			;
		bpo::options_description all;
		all.add(visible).add(invisible);
		bpo::positional_options_description pos_decr; pos_decr.add("network", 1);
		bpo::store(bpo::command_line_parser(argc, argv).options(all).positional(pos_decr).run(), result);

		if (result.count("help")) {
			cout << "tremppi_stat filename.json " << endl;
			cout << "   filename.json: a regulatory network file" << endl;
			cout << visible;
			exit(0);
		}

		if (result.count("version")) {
			cout << VERSION << "\n";
			exit(0);
		}

		bpo::notify(result);

		return result;
	}

	// Obtain the path to the input file
	bfs::path getNetworkPath(const bpo::variables_map & po) {
		bfs::path input_path = { po["network"].as<string>() };

		if (!bfs::exists(input_path))
			throw invalid_argument("Wrong filename \"" + input_path.string() + "\".\n");
		if (input_path.extension() != NETWORK_SUFFIX)
			throw invalid_argument("Network file \"" + input_path.string() + "\" must have suffix \"" + NETWORK_SUFFIX + "\".\n");

		return input_path;
	}
}

