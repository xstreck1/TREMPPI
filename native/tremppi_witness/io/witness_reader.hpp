#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/network/data_info.hpp>


class WitnessReader 
{
	unique_ptr<sqlite3pp::query> selection_qry;
	sqlite3pp::query::iterator sel_it;
	map<pair<string, string>, size_t> current_witness;

public:
	void select(const string & prop_name, const string & select, sqlite3pp::database & db);

	const map<pair<string, string>, size_t> & getWitness() const;

	bool next();
};