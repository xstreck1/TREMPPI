#include "program_options.hpp"

bpo::variables_map ProgramOptions::parseProgramOptions(int argc, char ** argv){
	bpo::variables_map result;

	// Declare the supbported options.
	visible.add_options()
		("version,v", "display version")
		("path", bpo::value<string>()->default_value(bfs::current_path().string()), "if not using the current execution folder, specifies the project folder.")
		("help,h", "display help");
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