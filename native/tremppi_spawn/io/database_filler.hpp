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

#pragma once

#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/database/database_reader.hpp>

#include "../data/model_translators.hpp"
#include "../data/kinetics_translators.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief A class that creates and fills a database with parametrizations.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DatabaseFiller 
{
	const Model & model;
	const Kinetics & kinetics;
	sqlite3pp::database db;

	bool in_output; ///< True if there is currently transaction ongoing.

	void prepareTable(const string & name, const string & columns);

	string makeInsert(const string & table);

	void fillComponents();	void fillRegulations();
	string getContexts() const;

	void fillParametrizations();

public:
	DatabaseFiller(const Model & _model, const Kinetics & _kinetics, const string & datafile_name);

	RegInfos creteTables();

	void dropTables();

	void addParametrization(const string & parametrization);

	void startOutput();
	void finishOutpout();
};
