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