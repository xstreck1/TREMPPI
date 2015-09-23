#include <json/json.h>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/report/report.hpp>
#include <tremppi_common/python/python_functions.hpp>

#include "compute/compute.hpp"
#include "io/output.hpp"

int tremppi_correlations(int argc, char ** argv) 
{
	TremppiSystem::initiate("tremppi_correlations", argc, argv);
	Logging logging;

	Json::Value out;
	RegInfos reg_infos;
	sqlite3pp::database db;
	vector<sqlite3pp::query> queries;
	try 
	{
		DEBUG_LOG << "Parsing data file.";
		// Read filter conditions
		out = Report::createSetup();

		db = move(sqlite3pp::database((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str()));

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
		for (const RegInfo & reg_info : reg_infos) 
		{
			map<size_t, string> columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("B_" + reg_info.name), db);
			if (columns.empty()) 
			{
				throw runtime_error("did not find the column B_" + reg_info.name);
			}
			queries.emplace_back(DatabaseReader::selectionFilter(columns, out["setup"]["select"].asString(), db));
		}
	}
	catch (exception & e) 
	{
		logging.exceptionMessage(e, 2);
	}

	FunsData funs_data;
	try 
	{
		DEBUG_LOG << "Computing function graph data.";
		Compute::deviation(reg_infos, out["setup"]["size"].asInt(), queries, logging, funs_data);
		Compute::correlation(reg_infos, out["setup"]["size"].asInt(), queries, logging, funs_data);
	}
	catch (exception & e) 
	{
		logging.exceptionMessage(e, 3);
	}
	try 
	{
		DEBUG_LOG << "Building the JSON.";
		out["elements"] = Output::functionalData(funs_data);
	}
	catch (exception & e) 
	{
		logging.exceptionMessage(e, 4);
	}

	try 
	{
		DEBUG_LOG << "Writing output.";
		FileManipulation::writeJSON(TremppiSystem::DATA_PATH / "correlations" / (out["setup"]["s_name"].asString() + ".json"), out);
	}
	catch (exception & e) 
	{
		logging.exceptionMessage(e, 5);
	}
	try
	{
		PythonFunctions::configure("correlations");
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 6);
	}
	return 0;
}