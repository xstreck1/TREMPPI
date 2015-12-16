/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "parametrization_reader.hpp"

void ParametrizationReader::select(const RegInfos & reg_infos, const string & selection, sqlite3pp::database & db) 
{
	vector<string> contexts = DataInfo::getAllContexts(reg_infos);
	if (contexts.empty()) 
	{
		throw runtime_error("No parameter columns found in the database.");
	}

	const string qry_string = "SELECT ROWID, " + boost::algorithm::join(contexts, ", ") + " FROM " + PARAMETRIZATIONS_TABLE + " WHERE " + selection;
	selection_qry.reset(new sqlite3pp::query(db, (qry_string).c_str()));
	sel_it = selection_qry->begin();
}

const Levels & ParametrizationReader::getParametrization() const 
{
	return current_parametrization;
}

// Iterate, return false if the query came to an end
bool ParametrizationReader::next() 
{
	if (sel_it == selection_qry->end())
	{
		return false;
	}

	current_rowid = sel_it->get<ParamNo>(0);
	current_parametrization = sqlite3pp::func::getRow<ActLevel>(*sel_it, 1, selection_qry->column_count() - 1);
	sel_it++;
	return true;
}

ParamNo ParametrizationReader::getRowID() const 
{
	return current_rowid;
}