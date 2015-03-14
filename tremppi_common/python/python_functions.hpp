#pragma once

#include "../header.h"

class PythonFunctions {
	bpy::object main_module;
	bpy::object main_namespace;

	PythonFunctions();
	PythonFunctions(const PythonFunctions &) = delete;
	PythonFunctions& operator=(const PythonFunctions&) = delete;
	PythonFunctions(PythonFunctions &&) = delete;
	PythonFunctions& operator=(PythonFunctions&&) = delete;
public:
	static PythonFunctions & getInstance();

	void exec(const string & command);


	static string reformPath(const bfs::path & path);

	template <typename ResultType>
	void eval(const string & command, typename ResultType & result);
};

template<typename ResultType>
void PythonFunctions::eval(const string & command, typename ResultType & result) {
	try {
		bpy::object value = bpy::eval(command.c_str(), main_namespace);
		result = bpy::extract<ResultType>(value);
	}
	catch (bpy::error_already_set const &)
	{
		PyErr_Print();
		throw std::runtime_error("Python exception encountered.");
	}
}