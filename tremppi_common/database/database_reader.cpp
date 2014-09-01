#include "database_reader.hpp"

ActLevel DatabaseReader::getMaxLevel(const string & name, sqlite3pp::database & db) {
	int result = 0;

	sqlite3pp::query qry(db, ("SELECT " + NAMES_COLUMN + ", " + MAX_LEVEL_COLUMN + " FROM " + COMPONENTS_TABLE).c_str());
	for (auto row : qry) {
		boost::tuple<string, int> data = row.get_columns<string, int>(0, 1);
		if (data.get<0>() == name)
			result = max(result, data.get<1>());
	}

	return static_cast<ActLevel>(result);
}

map<string, Levels> DatabaseReader::obtainRegulators(const string & component, sqlite3pp::database & db) {
	map<string, Levels>  result;

	sqlite3pp::query qry(db, ("SELECT * FROM " + REGULATIONS_TABLE).c_str());
	for (auto row : qry) {
		string source, target;
		ActLevel threshold;
		row.getter() >> target >> source >> threshold;
		if (target == component) {
			result[source].emplace_back(threshold);
		}
	}

	return result;
}

RegInfo DatabaseReader::readRegInfo(const CompID ID, const string & name, sqlite3pp::database & db) {
	ActLevel max = getMaxLevel(name, db);
	auto regulators = obtainRegulators(name, db);

	map<size_t, map<string, Levels> > requirements;
	auto columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("K_" + name + "_.*"), db);
	size_t prev_index = 0;
	for (const auto & column : columns) {
		if (column.first < prev_index) {
			throw runtime_error("Database format critical error. The columns for parameters are not in lexicographical order.");
		}
		prev_index = column.first;

		requirements.insert({ column.first, obtainRequirements(column.second, regulators, db) });
	}

	return RegInfo{ ID, name, max, move(columns), move(regulators), move(requirements)};
}

sqlite3pp::query DatabaseReader::selectionFilter(const map<size_t, string> & columns, const string & selection, sqlite3pp::database & db) {
	string columns_list = alg::join(DataConv::columns2list(columns), ", ");
	string where_clause = selection.empty() ? "" : " WHERE " + selection;
	return sqlite3pp::query(db, ("SELECT " + columns_list + " FROM " + PARAMETRIZATIONS_TABLE + where_clause).c_str());
}

sqlite3pp::query DatabaseReader::selectionIDs(const string & selection, sqlite3pp::database & db) {
	string where_clause = selection.empty() ? "" : " WHERE " + selection;
	return sqlite3pp::query(db, ("SELECT ROWID FROM " + PARAMETRIZATIONS_TABLE + where_clause).c_str());
}

map<string, Levels> DatabaseReader::obtainRequirements(const string & context, const map<string, Levels> & regulators, sqlite3pp::database & db) {
	map<string, Levels> result;

	const Levels thresholds = DataConv::getThrsFromContext(context);
	size_t reg_i = 0;
	for (auto & regul : regulators) {
		ActLevel low = thresholds[reg_i];
		auto high_it = find_if(WHOLE(regul.second), [low](const ActLevel ths){return ths > low; });
		ActLevel high = high_it == end(regul.second) ? getMaxLevel(regul.first, db) + 1 : *high_it;
		result.insert({ regul.first, vrange(low, high) });

		reg_i++;
	}

	return result;
}