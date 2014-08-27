#pragma once

#include <tremppi_common/header.h>
#include <tremppi_common/network/definitions.hpp>

namespace ExpressOptions {
	/* Parse the program options - if help or version is required, terminate the program immediatelly. */
	bpo::variables_map parseProgramOptions(int argc, char ** argv) {
		bpo::variables_map result;

		// Declare the supbported options.
		bpo::options_description visible("Execution options");
		visible.add_options()
			("help,h", "display help")
			("version,v", "display version")
			("select", bpo::value<string>(), "selection of the set of interest")
			;
		bpo::options_description invisible;
		invisible.add_options()
			("database", bpo::value<string>()->required(), ("Database file with " + DATABASE_SUFFIX + " suffix").c_str())
			;
		bpo::options_description all;
		all.add(visible).add(invisible);
		bpo::positional_options_description pos_decr; pos_decr.add("database", 1);
		bpo::store(bpo::command_line_parser(argc, argv).options(all).positional(pos_decr).run(), result);

		if (result.count("help")) {
			cout << tremppi_system.PROGRAM_NAME << " filename.sqlite " << endl;
			cout << "   filename.sqlite: a database with parametrizations." << endl;
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

	//
	string getFilter(const bfs::path & filter_path) {
		string select;

		if (!bfs::exists(filter_path))
			throw invalid_argument("Wrong filename \"" + filter_path.string() + "\".\n");
		if (filter_path.extension() != FILTER_SUFFIX)
			throw invalid_argument("Filter file \"" + filter_path.string() + "\" must have suffix \"" + FILTER_SUFFIX + "\".\n");

		ifstream filter_file(filter_path.string(), ios::in);
		string line;
		while (getline(filter_file, line))
			if (!line.empty())
				select += "(" + line + ") AND";

		// Remove the last end
		if (!select.empty())
			select.resize(select.size() - 4);

		return select;
	}

	//
	bfs::path getDatabasePath(const bpo::variables_map & po) {
		bfs::path input_path = { po["database"].as<string>() };

		if (!bfs::exists(input_path))
			throw invalid_argument("Wrong filename \"" + input_path.string() + "\".\n");
		if (input_path.extension() != DATABASE_SUFFIX)
			throw invalid_argument("Database file \"" + input_path.string() + "\" must have suffix \"" + DATABASE_SUFFIX + "\".\n");

		return input_path;
	}
}

