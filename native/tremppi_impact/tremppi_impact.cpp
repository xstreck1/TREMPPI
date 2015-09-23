#include <json/json.h>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/report/report.hpp>
#include <tremppi_common/python/python_functions.hpp>

#include "compute/impact_helpers.hpp"

int tremppi_impact(int argc, char ** argv) 
{
	TremppiSystem::initiate("tremppi_impact", argc, argv);
	Logging logging;

	bfs::path database_path = TremppiSystem::DATA_PATH / DATABASE_FILENAME;
	string selection;
	sqlite3pp::database db;
	RegInfos reg_infos;
	try 
	{
		// Get database
		db = move(sqlite3pp::database(database_path.string().c_str()));

		selection = DatabaseReader::getSelectionTerm();

		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
	}
	catch (exception & e) 
	{
		logging.exceptionMessage(e, 2);
	}

	// Label per parametrization
	try 
	{
		for (const RegInfo & reg_info : reg_infos) {
			for (const auto & regulator : reg_info.regulators) 
			{
				const string reg_name = reg_infos[regulator.first].name;
				for (const ActLevel & threshold : regulator.second) 
				{
					const string column_name = "I_" + reg_name + "_" + to_string(threshold) + "_" + reg_info.name;
					sqlite3pp::func::addColumn(PARAMETRIZATIONS_TABLE, column_name, "REAL", db);
				}
			}
		}
		logging.newPhase("Expressing component", reg_infos.size());
		for (const RegInfo & reg_info : reg_infos) 
		{
			// Skip if there are no edges
			if (reg_info.regulators.empty()) 
			{
				continue;
			}

			// Get the header data
			vector<string> columns_list = DataConv::columns2list(reg_info.columns);
			// For each regulator holds the values of the threshold in each column
			vector<Levels> reg_values;
			// For each regulator hold the columns that contain the contexts with this regulator and those exactly without is (i.e. if I have a context with this regulator and remove the regulator, what do I obtain?)
			map<CompID, vector<vector<size_t>>> edge_dist;
			for (const auto & regulator : reg_info.regulators) 
			{
				// Obtain the id of the regulator from the order in the regulator list
				size_t reg_i = DataInfo::RegIDToRegNo(reg_info, regulator.first);
				reg_values.emplace_back(DataConv::getThrFromContexts(columns_list, reg_i));
				edge_dist.insert({ regulator.first, ImpactHelpers::getColumnsOfRegulator(reg_i, regulator.second, reg_info.columns) });
			}

			// Select parametrizations and IDs
			sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, selection, db);
			sqlite3pp::query sel_IDs = DatabaseReader::selectionIDs(selection, db);
			sqlite3pp::query::iterator sel_it = sel_qry.begin();
			
			logging.newPhase("Listing parametrizations", DatabaseReader::getSelectionSize(selection, db));
			db.execute("BEGIN TRANSACTION");
			// Go through parametrizations
			for (auto sel_ID : sel_IDs) 
			{
				string update = "UPDATE " + PARAMETRIZATIONS_TABLE + " SET";
				Levels params = sqlite3pp::func::getRow<ActLevel>(*sel_it, 0, sel_qry.column_count());

				for (const size_t reg_no : cscope(reg_info.regulators)) 
				{
					const CompID reg_ID = DataInfo::RegNoToRegID(reg_info, reg_no);
					for (const size_t threshold_no : cscope(reg_info.regulators.at(reg_ID))) 
					{
						const ActLevel threshold = reg_info.regulators.at(reg_ID)[threshold_no];
						const double impact = Statistics::correlation(reg_values[reg_no], params, edge_dist.at(reg_ID)[threshold_no], reg_info.columns.begin()->first);
						update += " I_" + reg_infos[reg_ID].name + "_" + to_string(threshold) + "_" + reg_info.name + " = " + to_string(impact) + ",";
					}
				}
				update[update.size() - 1] = ' ';
				int rowid = sel_ID.get<int>(0);
				update += "WHERE ROWID=" + to_string(rowid);
				db.execute(update.c_str());
				logging.step();
				sel_it++;
			}
			db.execute("END");

			logging.step();
		}
	}
	catch (exception & e) 
	{
		logging.exceptionMessage(e, 3);
	}

	try
	{
		PythonFunctions::configure("select");
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 4);
	}

	return 0;
}