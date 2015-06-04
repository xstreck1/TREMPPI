#include "parametrization_reader.hpp"

void ParametrizationReader::select(const RegInfos & reg_infos, const string & selection, sqlite3pp::database & db) {
	vector<string> contexts = DataInfo::getAllContexts(reg_infos);
	if (contexts.empty())
		throw runtime_error("No parameter columns found in the database.");

	const string qry_string = "SELECT ROWID, " + boost::algorithm::join(contexts, ", ") + " FROM " + PARAMETRIZATIONS_TABLE + selection;
	selection_qry.reset(new sqlite3pp::query(db, (qry_string).c_str()));
	sel_it = selection_qry->begin();
}

const Levels & ParametrizationReader::getParametrization() const {
	return current_parametrization;
}

// Iterate, return false if the query came to an end

bool ParametrizationReader::next() {
	if (sel_it == selection_qry->end())
		return false;

	current_rowid = sel_it->get<ParamNo>(0);
	current_parametrization = sqlite3pp::func::getRow<ActLevel>(*sel_it, 1, selection_qry->column_count() - 1);
	sel_it++;
	return true;
}

ParamNo ParametrizationReader::getRowID() const {
	return current_rowid;
}
