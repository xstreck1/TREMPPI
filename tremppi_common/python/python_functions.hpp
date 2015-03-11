#pragma once

#include "../header.h"

class PythonFunctions {
	static PythonFunctions instance;
	bool initialized;

	PythonFunctions();
	PythonFunctions(const PythonFunctions &) = delete;
	PythonFunctions& operator=(const PythonFunctions&) = delete;
	PythonFunctions(PythonFunctions &&) = delete;
	PythonFunctions& operator=(PythonFunctions&&) = delete;
public:
	PythonFunctions & getInstance();
};