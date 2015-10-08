#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/header.h>
using namespace std;

map<string, pair<int(*)(int, char**), string> > tremppi_functions =
{
		{ "browse", { tremppi_browse, "open the IDE in the default browser" } },
		{ "configure", { tremppi_configure, "call to explicitly recreate data configuration files" } },
		{ "clean", { tremppi_clean, "removed all the data files" } },
		{ "bias", { tremppi_bias, "label parametrizations with their bias" } },
		{ "correlations", { tremppi_correlations, "create a correlations graph report"} },
		{ "cost", { tremppi_cost, "conduct a model check validate parametrizations agaings selected LTL properties" } },
		{ "express", { tremppi_express, "create the logical expressions for all the parametrizations" } },
		{ "impact", { tremppi_impact, "label with the impact of a regulator on its target" } },
		{ "init", { tremppi_init, "create a new model with a model viewer" } },
		{ "qualitative", { tremppi_qualitative, "get qualitative analysis of the known data" } },
		{ "quantitative", { tremppi_quantitative, "get summary of up till now known data" } },
		{ "regulations", { tremppi_regulations, "create an regulationsion graph based on a statistical analysis" } },
		{ "robustness", { tremppi_robustness, "conduct a model check and obtain robustness for selected properties" } },
		{ "sign", { tremppi_sign, "label the edges of individual with their signs" } },
		{ "spawn", { tremppi_spawn, "read a model and create a database of parametrizations based on the model" } },
		{ "trace", { tremppi_trace, "conduct a model check and obtain traces for satisfiable properties" } },
		{ "update", { tremppi_update, "update the javascript and html files in the project directory" } },
		{ "witness", { tremppi_witness, "produce a witness for the given LTL properties" } }
};


void printHelp()
{
	cout << "tremppi command [options]" << endl;
	for (const auto option : tremppi_functions)
	{
		cout << "\t" << option.first << ": " << option.second.second << endl;
	}
	cout << "\t--help,-h: display help for the specified command or display this dialog if no command is given\n";
}

void printVer()
{
	cout << "TREMPPI version " << VERSION << endl;
}

//
int main(int argc, char ** argv)
{
	TremppiSystem::standalone = false;
	if (argc < 2)
	{
		TremppiSystem::set("browse", argv[0], ".");
		if (!bfs::exists(TremppiSystem::HOME_PATH / PROJECTS_FOLDER)) {
			boost::filesystem::create_directory(TremppiSystem::HOME_PATH / PROJECTS_FOLDER);
		}
		vector<string> arguments = { "--path", bfs::absolute(TremppiSystem::HOME_PATH / PROJECTS_FOLDER).string() };
		ArgPtr arg_ptr(arguments);
		return (tremppi_browse(arg_ptr.getArgc(), arg_ptr.getArgv()));
	}
	string command = argv[1];
	if (command == "--help" || command == "-h")
	{
		printHelp();
		return 0;
	}
	if (command == "--ver" || command == "-v")
	{
		printVer();
		return 0;
	}
	else if (tremppi_functions.count(command) == 0)
	{
		cerr << "Fatal error: " << quote(command) << " is not a known program. Options are:\n";
		printHelp();
		return 1;
	}
	// Finally run the program
	else
	{
		// Move the program name to the second position instead of the command, which is forgotten 
		argv[1] = argv[0];
		return tremppi_functions.at(command).first(argc - 1, argv + 1);
	}
}
