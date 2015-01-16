#pragma once

#include "logging.hpp"
#include "time_manager.hpp"

// The global setup for Tremppi apps - mainly sets paths to the executables and data
namespace TremppiSystem {
	//
	void set(const char * name, const char * arg, const string & _WORK_PATH);

	extern string PROGRAM_NAME; // Name of the program running
	extern bfs::path HOME_PATH; // The path to the home of tremppi
	extern bfs::path WORK_PATH; // The directory in which tremppi works currently
	extern bfs::path EXEC_PATH; // The path from where the program was executed
	extern bfs::path BIN_PATH; // The path to the executed binary
	extern bool standalone; // True if executing the standalone application instead of the main launcher

	//
	template<typename OptionsT>
	bpo::variables_map initiate(const string & name, int argc, char **argv) {
		bpo::variables_map po;

		TimeManager::setStartTime();

		try {
			if (argc < 1)
				throw runtime_error("No parameters.");
			OptionsT options;
			po = options.parseProgramOptions(argc, argv);

			TremppiSystem::set(name.c_str(), argv[0], po["path"].as<string>());
		}
		catch (exception & e) {
			cerr << e.what() << endl;
			exit(1);
		}

		return po;
	}
}