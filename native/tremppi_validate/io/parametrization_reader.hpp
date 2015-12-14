#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/network/data_info.hpp>


class ParametrizationReader 
{
	unique_ptr<sqlite3pp::query> selection_qry;
	sqlite3pp::query::iterator sel_it;
	Levels current_parametrization;
	ParamNo current_rowid;
	vector<CompID> components_to_check;
	map<pair<size_t, bool>, int> neighbour_contexts; // For the purposes of normalization check, 


	// For the original regulatory context find a negibour context with the self regulation either higher (direction == true) or lower (direction == false)
	int find_neighbour(const Configurations & original, const bool direction, const RegInfo & reg_info);

	ActLevel normalize(const Levels & original, const RegInfo & reg_info, const size_t column, Levels & normalized);
public:
	ParametrizationReader(const RegInfos & reg_infos);

	void select(const RegInfos & reg_infos, const string & selection, sqlite3pp::database & db);

	const Levels & getParametrization() const;

	// Iterate, return false if the query came to an end
	bool next();

	ParamNo getRowID() const;

	bool isNormalized(const Levels & parametrization, const RegInfos & reg_infos);
};