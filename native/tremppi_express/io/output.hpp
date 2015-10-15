#pragma once

#include "../data/data.hpp"

namespace Output 
{
	// Add a function column for each component 
	void addColumns(const RegFuncs & reg_funcs, sqlite3pp::database & db);

	// 
	string plitToFormula(const string & component, const Levels & trhs, const PLit & plit, const ActLevel max);

	//
	string pminToFormula(const RegFuncs & funcs, const RegFunc & reg_func, const PMin & pmin, const ActLevel target_val);

	//
	string pdnfToFormula(const RegFuncs & funcs, const RegFunc & reg_func, const vector<PDNF> & pdnfs);
}