#include "python_functions.hpp"

PythonFunctions::PythonFunctions() {
	Py_Initialize();
}

PythonFunctions & PythonFunctions::getInstance()
{
	static PythonFunctions instance; 

	return instance;
}
