#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/general/logging.hpp>
#include <tremppi_common/header.h>
using namespace std;

int tremppi_spawn(int argc, char ** argv);
int tremppi_express(int argc, char ** argv);
int tremppi_report(int argc, char ** argv);
int tremppi_validate(int argc, char ** argv);
int tremppi_init(int argc, char ** argv);

map<string, pair<int(*)(int, char**), string> > tremppi_functions = {
		{ "spawn", { tremppi_spawn, "spawn" } },
		{ "express", { tremppi_express, "express" } },
		{ "report", { tremppi_report, "report" } },
		{ "valdiate", { tremppi_validate, "valdiate" } },
		{ "init", { tremppi_init, "init" } }
};

void printHelp() {
	cout << "tremppi function_name [options]" << endl;
	for (const auto option : tremppi_functions) {
		cout << "\t" << option.first << ": " << option.second.second << endl;
	}
	cout << "\thelp: display this dialog\n";
}

//
int main(int argc, char ** argv) {
	if (argc < 2) {
		cerr << "Fatal error: No program given on the input. Usage:\n";
		printHelp();
		return 1;
	}
	string command = argv[1];
	if (command == "help") {
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
		tremppi_system.standalone = false;
		// Move the program name to the second position instead of the command, which is forgotten 
		argv[1] = argv[0];
		tremppi_functions.at(command).first(argc - 1, argv + 1);
	}

	return 0;
}
