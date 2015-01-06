#pragma once

#include "logging.hpp"

// The global setup for Tremppi apps - mainly sets paths to the executables and data
class TremppiSystem {
public:
	string PROGRAM_NAME; // Name of the program running
	bfs::path HOME_PATH; // The path to the home of tremppi
	bfs::path WORK_PATH; // The directory in which tremppi works currently
	bfs::path EXEC_PATH; // The path from where the program was executed
	bfs::path BIN_PATH; // The path to the executed binary
	bool standalone; // True if executing the standalone application instead of the main launcher

	//
	void set(const char * name, const char * arg, const string & _WORK_PATH);

	//
	template<typename OptionsT>
	bpo::variables_map initiate(const string & name, int argc, char **argv) {
		bpo::variables_map po;

		try {
			if (argc < 1)
				throw runtime_error("No parameters.");
			OptionsT options;
			po = options.parseProgramOptions(argc, argv);

			string name = (standalone) ? PROGRAM_NAME : "tremppi";
			logging.init(WORK_PATH, name);
			BOOST_LOG_TRIVIAL(info) << tremppi_system.PROGRAM_NAME << " started.";

			set(name.c_str(), argv[0], po["path"].as<string>());
		}
		catch (exception & e) {
			cerr << e.what() << endl;
			exit(1);
		}

		return po;
	}
};

extern TremppiSystem tremppi_system;