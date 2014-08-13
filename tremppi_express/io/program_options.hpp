#pragma once

#include <tremppi_common/header.h>
#include <tremppi_common/general/common_functions.hpp>

const string PROGRAM_NAME = "tremppi_express";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief User options interpretation. Parses the options, terminates on --version and --help.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bpo::variables_map parseProgramOptions(int argc, char ** argv) {
	bpo::variables_map result;

	// Declare the supbported options.
	bpo::options_description visible("Execution options");
	visible.add_options();
	bpo::options_description invisible;
	invisible.add_options()
		("database", bpo::value<string>()->required(), ("Database file with " + DATABASE_SUFFIX + " suffix").c_str())
		;
	bpo::options_description all;
	all.add(visible).add(invisible);
	bpo::positional_options_description pos_decr; pos_decr.add("database", 1);
	bpo::store(bpo::command_line_parser(argc, argv).options(all).positional(pos_decr).run(), result);

	if (result.count("help")) {
		cout << "Minimizer filename.sqlite" << endl;
		cout << "   filename.sqlite: a database with parametrizations." << endl;
		cout << visible;
		exit(0);
	}

	if (result.count("version")) {
		cout << VERSION << "\n";
		exit(0);
	}

	bpo::notify(result);
	if (result.count("select") < result.count("compare"))
		throw runtime_error(quote("compare") + " program option requires the " + quote("select") + " to be set too.");

	return result;
}