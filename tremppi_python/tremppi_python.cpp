#include <boost/python/detail/wrap_python.hpp>
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/general/logging.hpp>
#include <tremppi_common/header.h>
using namespace std;

int tremppi_python(const string command, int argc, char ** argv) {
	Py_SetProgramName(argv[0]); 

	bfs::path file_path = tremppi_system.HOME_PATH / bfs::path{ "python" } / bfs::path{ command } / bfs::path{ command + ".py" };
	if (!bfs::exists(file_path)) {
		cerr << file_path.string() << " is called, but does not exist.";
		return 1;
	}

	int passed_argc;
	char * passed_argv[3];

	passed_argc = 2;
	passed_argv[0] = new char[file_path.string().size() + 1];
	strcpy(passed_argv[0], file_path.string().c_str());

	Py_SetProgramName(argv[0]);
	Py_Initialize();
	PySys_SetArgv(passed_argc, passed_argv);

	PyObject* PyFileObject = PyFile_FromString(passed_argv[0], "r");
	PyRun_SimpleFileEx(PyFile_AsFile(PyFileObject), passed_argv[0], 1);

	Py_Finalize(); 

	return 0;
}

int tremppi_init(int argc, char ** argv) {
	tremppi_system.set("tremppi", argv[0], bfs::path{ "." });
	return tremppi_python("init", argc, argv);
}
