#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/header.h>
using namespace std;

map<string, pair<int(*)(int, char**), string> > tremppi_functions = {
		{ "init", { tremppi_init, "create a new model with a model viewer" } },
		{ "browse", { tremppi_browse, "starts a local server to allow browsing data" } },
		{ "configure", { tremppi_configure, "call to explicitly recreate data configuration files" } },
		{ "correlate",{ tremppi_correlate, "create a function graph based on a statistical analysis" } },
		{ "express", { tremppi_express, "create the logical expressions for all the parametrizations" } },
		{ "qualitative", { tremppi_qualitative, "get qualitative analysis of the known data" } },
		{ "quantitative", { tremppi_quantitative, "get summary of up till now known data" } },
		{ "regulations",{ tremppi_regulations, "create an regulationsion graph based on a statistical analysis" } },
		{ "spawn",{ tremppi_spawn, "read a model and create a database of parametrizations based on the model" } },
		{ "update", { tremppi_update, "update the javascript and html files in the project directory" } },
		{ "validate", { tremppi_validate, "conduct a model checking to validate parametrizations agaings LTL properties" } },
		{ "witness", { tremppi_witness, "produce a witness for the given LTL properties (needs valiation first)" } }
};


void printHelp() {
	cout << "tremppi command [options]" << endl;
	for (const auto option : tremppi_functions) {
		cout << "\t" << option.first << ": " << option.second.second << endl;
	}
	cout << "\t--help,-h: display help for the specified command or display this dialog if no command is given\n";
}

//
int main(int argc, char ** argv) {
	TremppiSystem::standalone = false;

	if (argc < 2) {
		cerr << "Fatal error: No program given on the input. Usage:\n";
		printHelp();
		return 1;
	}
	string command = argv[1];
	if (command == "--help" || command == "-h") {
		printHelp();
		return 0;
	}
	else if (tremppi_functions.count(command) == 0) {
		cerr << "Fatal error: " << quote(command) << " is not a known program. Options are:\n";
		printHelp();
		return 1;
	}
	// Finally run the program
	else {
		// Move the program name to the second position instead of the command, which is forgotten 
		argv[1] = argv[0];
		return tremppi_functions.at(command).first(argc - 1, argv + 1);
	}
}
