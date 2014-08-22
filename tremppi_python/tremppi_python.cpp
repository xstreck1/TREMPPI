#include <boost/python/detail/wrap_python.hpp>
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/general/logging.hpp>
#include <tremppi_common/header.h>
using namespace std;

int tremppi_python(const string command, int argc, char ** argv) {
	// Check if the script exists
	bfs::path file_path = tremppi_system.HOME_PATH / bfs::path{ "python" } / bfs::path{ command } / bfs::path{ command + ".py" };
	if (!bfs::exists(file_path)) {
		cerr << file_path.string() << " is called, but does not exist.";
		return 1;
	}
	// Copy the script path to the first argument
	char * program_name = new char[file_path.string().size() + 1];
	strcpy(program_name, file_path.string().c_str());
	swap(program_name, argv[0]);

	// Initialize Python interpreter, the intepreter gets the name of this binary as the argv[0]
	Py_SetProgramName(program_name);
	Py_Initialize();
	PySys_SetArgv(argc, argv);

	// Run python
	PyObject* PyFileObject = PyFile_FromString(argv[0], "r");
	PyRun_SimpleFileEx(PyFile_AsFile(PyFileObject), argv[0], 1);

	Py_Finalize(); 

	swap(program_name, argv[0]);
	delete[] program_name;
	return 0;
}

int tremppi_init(int argc, char ** argv) {
	tremppi_system.set("tremppi", argv[0], bfs::path{ "." });
	return tremppi_python("init", argc, argv);
}
