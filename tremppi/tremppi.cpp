#include <tremppi_common/general/logging.hpp>
#include <boost/python/detail/wrap_python.hpp>
using namespace std;

//
int main(int argc, char ** argv) {
	if (argc < 2)
		"No program given on the input";

	Py_SetProgramName(argv[0]);  /* optional but recommended */
	tremppi_system.set("tremppi", argv[0], bfs::path{"."});
	Logging::phase_count = 1;
	Logging::init(tremppi_system.PROGRAM_NAME + ".log");
	BOOST_LOG_TRIVIAL(info) << "TREMPPI core started.";

	string program_name = argv[1];
	bfs::path file_path = tremppi_system.HOME_PATH / bfs::path{ "tremppi_init" } / bfs::path{ "init.py" };

	FILE* file;
	int passed_argc;
	char * passed_argv[3];

	passed_argc = 3;
	passed_argv[0] = new char[file_path.stem().string().size() + 1];
	strcpy(passed_argv[0], file_path.stem().string().c_str());
	passed_argv[1] = "-m";
	passed_argv[2] = "/tmp/targets.list";

	Py_SetProgramName(argv[0]);
	Py_Initialize();
	PySys_SetArgv(passed_argc, passed_argv);
	file = fopen(file_path.string().c_str(), "r");
	if (file == NULL)
		cout << "Not open";
	cout << file_path.string() << endl;
	try {
		PyRun_SimpleString("print('Python Print test')");
	}
	catch (exception & e) {
		cout << e.what();
	}
	Py_Finalize();

	return 0;
}
