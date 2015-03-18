#include "database_reader.hpp"
#include "../general/system.hpp"
#include "../general/file_manipulation.hpp"
#include "../python/python_functions.hpp"

map<CompID, Levels> DatabaseReader::readRegulators(const string & component, sqlite3pp::database & db) {
	map<CompID, Levels>  result;

	sqlite3pp::query qry(db, ("SELECT * FROM " + REGULATIONS_TABLE).c_str());
	for (auto row : qry) {
		string source, target; ActLevel threshold;
		row.getter() >> source >> target >> threshold;
		if (target == component) {
			result[components_dict[source]].emplace_back(threshold);
		}
	}

	// Make sure the thresholds are sorted
	for (auto & regulator : result) 
		sort(WHOLE(regulator.second));

	return result;
}

void DatabaseReader::readMaxes(sqlite3pp::database & db) {
	sqlite3pp::query qry(db, ("SELECT " + NAMES_COLUMN + ", " + MAX_LEVEL_COLUMN + " FROM " + COMPONENTS_TABLE).c_str());
	
	maxes.resize(components.size());
	for (auto row : qry) {
		string component; ActLevel max_activity;
		row.getter() >> component >> max_activity;
		maxes_dict.insert({ component, max_activity });
		maxes[components_dict[component]] = max_activity;
	}
}

void DatabaseReader::readComponents(sqlite3pp::database & db) {
	// Read regulatory information
	sqlite3pp::query qry(db, ("SELECT * FROM " + COMPONENTS_TABLE).c_str());

	for (auto row : qry) 
		components.push_back(row.get<string>(0));
	sort(WHOLE(components));

	for (CompID ID = 0; ID < components.size(); ID++)
		components_dict.insert({ components[ID], ID });
}

RegInfos DatabaseReader::readRegInfos(sqlite3pp::database & db) {
	RegInfos result;
	readComponents(db);
	readMaxes(db);

	for (const string & component : components) {
		map<CompID, Levels> requlators = readRegulators(component, db);

		// read requirements for each context
		map<size_t, vector<Levels> > requirements;
		auto columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("K_" + component + "_.*"), db);
		size_t prev_index = 0;
		for (const auto & column : columns) {
			if (column.first < prev_index) {
				throw runtime_error("Database format critical error. The columns for parameters are not in lexicographical order.");
			}
			prev_index = column.first;

			requirements.insert({ column.first, obtainRequirements(column.second, requlators, db) });
		}

		result.push_back({ components_dict[component], component, maxes_dict[component], move(columns), move(requlators), requirements });
	}
	return result;
}

vector<Levels> DatabaseReader::obtainRequirements(const string & context, const map<CompID, Levels> & regulators, sqlite3pp::database & db) {
	vector<Levels> result(components.size(), Levels());

	const Levels thresholds = DataConv::getThrsFromContext(context);
	size_t reg_i = 0;
	for (const CompID ID : cscope(components)) {
		// If the component is not a regulator, all values are possible
		if (regulators.count(ID) < 1) {
			result[ID] = vrange<ActLevel>(0, maxes[ID] + 1);
		} else {
			ActLevel low = thresholds[reg_i];
			auto high_it = find_if(WHOLE(regulators.at(ID)), [low](const ActLevel ths){return ths > low; });
			ActLevel high = high_it == end(regulators.at(ID)) ? maxes[ID] + 1 : *high_it;
			result[ID] = vrange<ActLevel>(low, high);

			reg_i++;
		}
	}

	return result;
}

sqlite3pp::query DatabaseReader::selectionFilter(const map<size_t, string> & columns, const string & selection, sqlite3pp::database & db) {
	string columns_list = alg::join(DataConv::columns2list(columns), ", ");
	return sqlite3pp::query(db, ("SELECT " + columns_list + " FROM " + PARAMETRIZATIONS_TABLE + selection).c_str());
}

sqlite3pp::query DatabaseReader::selectionIDs(const string & selection, sqlite3pp::database & db) {
	return sqlite3pp::query(db, ("SELECT ROWID FROM " + PARAMETRIZATIONS_TABLE + selection).c_str());
}

string DatabaseReader::getSelectionTerm() {
	string result;

	PythonFunctions & python = PythonFunctions::getInstance();
	python.exec("from tremppi.select import selection_from_file");
	string selection;
	python.eval<string>("selection_from_file(" + PythonFunctions::reformPath(bfs::absolute(bfs::path{ TremppiSystem::HOME_PATH } / "javascript" / "select.json")) + ")", selection);

	return result;
}
