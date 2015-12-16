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

#include "database_filler.hpp"

void DatabaseFiller::prepareTable(const string & name, const string & columns)
{
	// Drop old tables if any.
	string drop_cmd = "DROP TABLE IF EXISTS " + name + "; ";
	string create_cmd = "CREATE TABLE " + name + " " + columns + ";\n";
	db.execute((drop_cmd + create_cmd).c_str());
}

string DatabaseFiller::makeInsert(const string & table)
{
	return "INSERT INTO " + table + " VALUES ";
}

void DatabaseFiller::fillComponents()
{
	prepareTable(COMPONENTS_TABLE, "(Name TEXT, MaxActivity INTEGER)");

	string update = "";
	for (CompID t_ID : crange(model.components.size()))
	{
		string values = "(\"" + model.components[t_ID].name + "\", " + to_string(model.components[t_ID].max_activity) + "); \n";
		update += makeInsert(COMPONENTS_TABLE) + values;
	}
	db.execute(update.c_str());

}
void DatabaseFiller::fillRegulations()
{
	prepareTable(REGULATIONS_TABLE, "(Source TEXT, Target TEXT, Threshold INTEGER)");
	vector<string> updates;
	for (CompID t_ID : crange(model.components.size()))
	{
		for (auto regul : ModelTranslators::getThresholds(model, t_ID)) {
			for (ActLevel threshold : regul.second)
			{
				string values = "(";
				values += quote(model.components[regul.first].name) + ", ";
				values += quote(model.components[t_ID].name) + ", ";
				values += quote(threshold);
				values += ");\n";
				updates.push_back(makeInsert(REGULATIONS_TABLE) + values);
			}
		}
	}
	// Sort so the values in the db are also sorted
	sort(WHOLE(updates));
	for (const string & update : updates)
	{
		db.execute(update.c_str());
	}
}
string DatabaseFiller::getContexts() const
{
	string contexts = "";
	for (CompID t_ID : crange(model.components.size()))
		for (auto & param : kinetics.components[t_ID].params)
			contexts += KineticsTranslators::makeConcise(param, model.components[t_ID].name) + " INTEGER, ";
	contexts.resize(contexts.size() - 2); // Remove ", " at the end
	return contexts;
}

void DatabaseFiller::fillParametrizations()
{
	string columns = "(" + getContexts() + ")";

	prepareTable(PARAMETRIZATIONS_TABLE, columns);
}

DatabaseFiller::DatabaseFiller(const Model & _model, const Kinetics & _kinetics, const string & datafile_name) : model(_model), kinetics(_kinetics)
{
	db = sqlite3pp::database(datafile_name.c_str());
	in_output = false;
}

RegInfos DatabaseFiller::creteTables()
{
	db.execute("BEGIN TRANSACTION;");
	fillComponents();
	fillRegulations();
	fillParametrizations();
	db.execute("END;");

	DatabaseReader reader;
	return reader.readRegInfos(db);
}

void DatabaseFiller::dropTables()
{
	const string DROP_CMD("DROP TABLE ");
	db.execute((DROP_CMD + COMPONENTS_TABLE + ";").c_str());
	db.execute((DROP_CMD + REGULATIONS_TABLE + ";").c_str());
	db.execute((DROP_CMD + PARAMETRIZATIONS_TABLE + ";").c_str());
}

void DatabaseFiller::addParametrization(const string & parametrization)
{
	auto insert = makeInsert(PARAMETRIZATIONS_TABLE);
	db.execute((insert + parametrization).c_str());
}

void DatabaseFiller::startOutput()
{
	db.execute("BEGIN TRANSACTION;");
	in_output = true;
}
void DatabaseFiller::finishOutpout()
{
	if (in_output)
		db.execute("END;");
	in_output = false;
}
