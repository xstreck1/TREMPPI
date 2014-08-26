#pragma once

#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/network/model_translators.hpp>
#include <tremppi_common/network/kinetics_translators.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief A class that creates and fills a database with parametrizations.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DatabaseFiller {
	const Model & model;
	const Kinetics & kinetics;
	sqlite3pp::database db;

	bool in_output; ///< True if there is currently transaction ongoing.

	void prepareTable(const string & name, const string & columns) {
		// Drop old tables if any.
		string drop_cmd = "DROP TABLE IF EXISTS " + name + "; ";
		string create_cmd = "CREATE TABLE " + name + " " + columns + ";\n";
		db.execute((drop_cmd + create_cmd).c_str());
	}

	inline string makeInsert(const string & table) {
		return "INSERT INTO " + table + " VALUES ";
	}

	void fillComponents() {
		prepareTable(COMPONENTS_TABLE, "(Name TEXT, MaxActivity INTEGER)");

		string update = "";
		for (CompID t_ID : crange(model.components.size())) {
			string values = "(\"" + model.components[t_ID].name + "\", " + to_string(model.components[t_ID].max_activity) + "); \n";
			update += makeInsert(COMPONENTS_TABLE) + values;
		}
		db.execute(update.c_str());

	}

	void fillRegulations() {
		prepareTable(REGULATIONS_TABLE, "(Target TEXT, Source TEXT, Threshold INTEGER)");
		string update = "";
		for (CompID t_ID : crange(model.components.size())) {
			for (auto regul : ModelTranslators::getThresholds(model, t_ID)) {
				for (ActLevel threshold : regul.second) {
					string values = "(";
					values += quote(model.components[t_ID].name) + ", ";
					values += quote(model.components[regul.first].name) + ", ";
					values += quote(threshold);
					values += ");\n";
					update += makeInsert(REGULATIONS_TABLE) + values;
				}
			}
		}
		db.execute(update.c_str());
	}

	string getContexts() const {
		string contexts = "";
		for (CompID t_ID : crange(model.components.size()))
			for (auto param : kinetics.components[t_ID].params)
				contexts += KineticsTranslators::makeConcise(param, model.components[t_ID].name) + " INTEGER, ";
		contexts.resize(contexts.size() - 2); // Remove ", " at the end
		return contexts;
	}

	void fillParametrizations() {
		string columns = "(" + getContexts() + ")";

		prepareTable(PARAMETRIZATIONS_TABLE, columns);
	}

public:
	DatabaseFiller(const Model & _model, const Kinetics & _kinetics, const string & datafile_name) : model(_model), kinetics(_kinetics) {
		db = sqlite3pp::database(datafile_name.c_str());
		in_output = false;
	}

	void creteTables() {
		db.execute("BEGIN TRANSACTION;");
		fillComponents();
		fillRegulations();
		fillParametrizations();
		db.execute("END;");
	}

	void dropTables() {
		const string DROP_CMD("DROP TABLE ");
		db.execute((DROP_CMD + COMPONENTS_TABLE + ";").c_str());
		db.execute((DROP_CMD + REGULATIONS_TABLE + ";").c_str());
		db.execute((DROP_CMD + PARAMETRIZATIONS_TABLE + ";").c_str());
	}

	void addParametrization(const string & parametrization) {
		auto insert = makeInsert(PARAMETRIZATIONS_TABLE);
		db.execute((insert + parametrization).c_str());
	}

	void startOutput() {
		db.execute("BEGIN TRANSACTION;");
		in_output = true;
	}

	void finishOutpout() {
		if (in_output)
			db.execute("END;");
		in_output = false;
	}
};
