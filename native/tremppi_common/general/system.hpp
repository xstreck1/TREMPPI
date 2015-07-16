#pragma once

#include "logging.hpp"
#include "time_manager.hpp"

// The global setup for Tremppi apps - mainly sets paths to the executables and data

namespace TremppiSystem 
{
	//
	void set(const char * name, const char * arg, const string & _WORK_PATH);

	//
	void initiate(const string & name, int argc, char **argv);

	extern string PROGRAM_NAME; // Name of the program running
	extern bfs::path HOME_PATH; // The path to the home of tremppi
	extern bfs::path WORK_PATH; // The directory in which tremppi works currently
	extern bfs::path EXEC_PATH; // The path from where the program was executed
	extern bfs::path BIN_PATH; // The path to the executed binary
	extern bfs::path DATA_PATH; // The path to the data failes
	extern bool standalone; // True if executing the standalone application instead of the main launcher
}