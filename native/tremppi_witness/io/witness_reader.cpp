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

#include "witness_reader.hpp"

void WitnessReader::select(const string & prop_name, const string & select, sqlite3pp::database & db)
{
	const string column_name = "W_" + prop_name;
	const string qry_string = "SELECT " + column_name + " FROM " + PARAMETRIZATIONS_TABLE + " WHERE " + select;
	selection_qry.reset(new sqlite3pp::query(db, (qry_string).c_str()));
	sel_it = selection_qry->begin();
}


const map<pair<string, string>, size_t> & WitnessReader::getWitness() const
{
	return current_witness;
}

bool WitnessReader::next()
{
	if (sel_it == selection_qry->end())
		return false;
	current_witness.clear();
	string witness_string = sel_it->get<string>(0);


	if (witness_string != "")
	{
		vector<string> transitions;
		boost::split(transitions, witness_string, is_any_of(";"));

		for (const string & transition : transitions)
		{
			vector<string> nodes(2);
			boost::split(nodes, transition, is_any_of(">"));
			current_witness[make_pair(nodes[0], nodes[1])]++;
		}
	}

	sel_it++;
	return true;
}