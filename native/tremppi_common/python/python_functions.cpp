#include "python_functions.hpp"
#include "../general/system.hpp"


PythonFunctions::PythonFunctions() 
{
	Py_Initialize();
	main_module = bpy::import("__main__");
	main_namespace = main_module.attr("__dict__");
	exec("import sys");
	string append_cmd = "sys.path.append(" + reformPath(TremppiSystem::HOME_PATH / "python") + ")";
	exec(append_cmd);
}


string PythonFunctions::reformPath(const bfs::path & path) 
{
	return "'" + std::regex_replace(path.string(), std::regex("\\\\"), "/") + "'";
}


PythonFunctions & PythonFunctions::getInstance()
{
	static PythonFunctions instance; 
	return instance;
}


void PythonFunctions::exec(const string & command) 
{
	try {
		bpy::exec(command.c_str(), main_namespace);
	}
	catch (bpy::error_already_set const &)
	{
		throw_python_exception();
	}
}


void PythonFunctions::configure(string widget) 
{
	PythonFunctions & python = PythonFunctions::getInstance();
	python.exec("from tremppi.configure import configure");
	python.exec("configure(" + PythonFunctions::reformPath(bfs::absolute(TremppiSystem::DATA_PATH)) + ", \"" + widget + "\")");
}

void PythonFunctions::throw_python_exception()
{
	PyObject *exc, *val, *tb;
	bpy::object formatted_list, formatted;
	PyErr_Fetch(&exc, &val, &tb);
	bpy::handle<> hexc(exc), hval(bpy::allow_null(val)), htb(bpy::allow_null(tb));
	bpy::object traceback(bpy::import("traceback"));
	if (!tb) {
		bpy::object format_exception_only(traceback.attr("format_exception_only"));
		formatted_list = format_exception_only(hexc, hval);
	}
	else {
		bpy::object format_exception(traceback.attr("format_exception"));
		formatted_list = format_exception(hexc, hval, htb);
	}
	formatted = bpy::str("\n").join(formatted_list);
	throw std::runtime_error(string(bpy::extract<std::string>(formatted)));
}
