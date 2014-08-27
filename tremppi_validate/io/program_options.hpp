#pragma once

#include <tremppi_common/header.h>
#include <tremppi_common/network/definitions.hpp>

enum TraceType { none, rob, wit }; //< What sort of trace the user requests - no trace, computation of robustness only or the transitions of the trace
enum BoundType { inf, min, step }; //< What sort of bound the user placed - do the full check / check until the minimal lenght is found for a parametrization / use only bounded amount of steps

namespace ValidateOptions {

	/* Parse the program options - if help or version is required, terminate the program immediatelly. */
	bpo::variables_map parseProgramOptions(int argc, char ** argv) {
		bpo::variables_map result;

		// Declare the supbported options.
		bpo::options_description visible("Execution options");
		visible.add_options()
			("help,h", "display help")
			("version,v", "display version")
			("select", bpo::value<string>()->default_value("1"), "selection of the set of interest")
			("trace", bpo::value<string>()->default_value("none"), "what form of trace analysis should be applied, one of {none, rob, wit}")
			("bound", bpo::value<string>()->default_value("inf"), "what bound is placed on the search, one of {inf, min, N} where N is a non-zero positive integer")
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

	// @obtains the exact numeric bound from the parsed options. If the bound can't be obtained, return INF
	size_t getBound(const bpo::variables_map & po) {
		if (regex_match(po["bound"].as<string>(), regex("\\d+")))
			return po["bound"].as<size_t>();
		else
			return INF;
	}

	// @brief:	obtains the trace type from the parsed options, also does syntax check
	TraceType getTracteType(const bpo::variables_map & po)  {
		string input = po["trace"].as<string>();
		if (input == "none")
			return TraceType::none;
		else if (input == "rob")
			return TraceType::rob;
		else if (input == "wit")
			return TraceType::wit;
		else
			throw runtime_error("Unknown trace type specified: " + input);
	}

	// @brief:	obtains the bound type from the parsed options, also does syntax check
	BoundType getBoundType(const bpo::variables_map & po) {
		string input = po["bound"].as<string>();
		if (input == "inf")
			return BoundType::inf;
		else if (input == "min")
			return BoundType::min;
		else if (regex_match(input, regex("\\d+")))
			return BoundType::step;
		else
			throw runtime_error("Unknown bound type specified: " + input);
	}
}
