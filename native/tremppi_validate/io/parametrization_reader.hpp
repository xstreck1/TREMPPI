#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/network/data_info.hpp>


class ParametrizationReader 
{
	unique_ptr<sqlite3pp::query> selection_qry;
	sqlite3pp::query::iterator sel_it;
	Levels current_parametrization;
	ParamNo current_rowid;
public:
	void select(const RegInfos & reg_infos, const string & selection, sqlite3pp::database & db);

	const Levels & getParametrization() const;

	// Iterate, return false if the query came to an end
	bool next();

	ParamNo getRowID() const;
};