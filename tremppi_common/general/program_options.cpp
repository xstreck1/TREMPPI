#include "program_options.hpp"

bpo::variables_map ProgramOptions::parseProgramOptions(int argc, char ** argv){
	bpo::variables_map result;

	// Declare the supbported options.
	visible.add_options()
		("version,v", "display version")
		("path", bpo::value<string>()->default_value(bfs::current_path().string()), "if not using the current execution folder, specifies path to the networks.js file")
		("help,h", "display help");
		;
	all.add(visible).add(invisible);
	bpo::store(bpo::command_line_parser(argc, argv).options(all).run(), result);

	if (result.count("help")) {
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

bfs::path ProgramOptions::getPath(const bpo::variables_map & po, const string & filename) {
	bfs::path file_path = bfs::path{ po["path"].as<string>() } / filename;

	if (!bfs::exists(file_path))
		throw invalid_argument(file_path.string() + "does not exists");

	return bfs::path{ po["path"].as<string>() };
}

//
string ProgramOptions::getFilter(const bfs::path & filter_path) {
	string select;

	if (!bfs::exists(filter_path))
		throw invalid_argument("Wrong filename \"" + filter_path.string() + "\".\n");
	if (filter_path.extension() != ".filter")
		throw invalid_argument("Filter file \"" + filter_path.string() + "\" must have suffix \"" + ".filter" + "\".\n");

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