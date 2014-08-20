#include <boost/python/detail/wrap_python.hpp>
#include <tremppi_common/header.h>
using namespace std;

int tremppi_spawn(int argc, char ** argv);
int tremppi_express(int argc, char ** argv);
int tremppi_report(int argc, char ** argv);
int tremppi_validate(int argc, char ** argv);

int tremppi_python(int argc, char ** argv) {
	/* Py_SetProgramName(argv[0]); 
	tremppi_system.set("tremppi", argv[0], bfs::path{ "." });

	string program_name = argv[1];
	bfs::path file_path = tremppi_system.HOME_PATH / bfs::path{ "tremppi_init" } / bfs::path{ "init.py" };

	FILE* file;
	int passed_argc;
	char * passed_argv[3];

	passed_argc = 2;
	passed_argv[0] = new char[file_path.string().size() + 1];
	strcpy(passed_argv[0], file_path.string().c_str());
	passed_argv[1] = "test";

	Py_SetProgramName(argv[0]);
	Py_Initialize();
	PySys_SetArgv(passed_argc, passed_argv);

	PyObject* PyFileObject = PyFile_FromString(passed_argv[0], "r");
	PyRun_SimpleFileEx(PyFile_AsFile(PyFileObject), passed_argv[0], 1);

	Py_Finalize(); */

	return 0;
}

map<string, pair<int(*)(int, char**), string> > tremppi_functions = {
		{ "python", { tremppi_python, "python" } },
		{ "spawn", { tremppi_spawn, "spawn" } },
		{ "express", { tremppi_express, "express" } },
		{ "report", { tremppi_report, "report" } },
		{ "valdiate", { tremppi_report, "valdiate" } }
};

void printHelp() {
	cout << "tremppi function_name [options]" << endl;
	cout << "\t spawn: display ";

}

//
int main(int argc, char ** argv) {
	if (argc < 2) {
		cerr << "No program given on the input";
		printHelp();
		return 1;
	}

	return 0;
}
