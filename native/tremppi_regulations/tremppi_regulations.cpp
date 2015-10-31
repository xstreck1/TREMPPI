#include <json/json.h>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/report/report.hpp>
#include <tremppi_common/python/python_functions.hpp>

#include "compute/regulatory_graph.hpp"
#include "compute/edge_signs.hpp"
#include "io/output.hpp"
//
int tremppi_regulations(int argc, char ** argv)
{
	TremppiSystem::initiate("tremppi_regulations", argc, argv);
	Logging logging;
	RegInfos reg_infos;
	sqlite3pp::database db;
	vector<string> selections;
	vector<string> sels_name;
	DatabaseReader reader;
	try 
	{
		DEBUG_LOG << "Reading data.";
		db = move(sqlite3pp::database((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str()));
		selections = reader.getSelectionList();
		sels_name = reader.getSelectionNames();
		reg_infos = reader.readRegInfos(db);
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 2);
	}

	// Create a report for each selection
	for (const size_t sel_no : cscope(selections))
	{
		Json::Value out;

		try
		{
			DEBUG_LOG << "Selection " + sels_name[sel_no];
			out = Report::createSetup(selections[sel_no], sels_name[sel_no]);

			if (sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("I_.*"), db).empty()) {
				throw runtime_error("Impact columns not available in the database.");
			}
			if (sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("S_.*"), db).empty()) {
				throw runtime_error("Sign columns not available in the database.");
			}
		}
		catch (exception & e)
		{
			logging.exceptionMessage(e, 3);
		}

		RegsData regs_data;
		try
		{
			DEBUG_LOG << "Computing regulationsion graph data.";
			logging.newPhase("Harvesting component", reg_infos.size());

			for (const RegInfo & reg_info : reg_infos)
			{
				regs_data.emplace_back(RegData(reg_info));
				RegData & reg_data = regs_data.back();
				map<size_t, string> impact_columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("I_.*" + reg_info.name), db);
				if (!impact_columns.empty())
				{
					sqlite3pp::query impact_qry = DatabaseReader::selectionFilter(impact_columns, out["setup"]["select"].asString(), db);
					RegulatoryGraph::compute(reg_infos, reg_info.ID, out["setup"]["size"].asInt(), impact_qry, reg_data.reg_corr);
				}

				EdgeSigns::computeExpectedFreq(reg_info, reg_data.expected_freq);
				map<size_t, string> sign_columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("S_.*" + reg_info.name), db);
				if (!sign_columns.empty())
				{
					sqlite3pp::query label_qry = DatabaseReader::selectionFilter(sign_columns, out["setup"]["select"].asString(), db);
					EdgeSigns::compute(reg_infos, reg_info.ID, out["setup"]["size"].asInt(), label_qry, reg_data.reg_freq, reg_data.reg_sign);
				}
				logging.step();
			}
		}
		catch (exception & e)
		{
			logging.exceptionMessage(e, 4);
		}

		try
		{
			DEBUG_LOG << "Writing results.";
			// For each graph create the graph data and add configuration details
			out["elements"] = Output::regulatoryGraph(reg_infos, regs_data);

			FileManipulation::writeJSON(TremppiSystem::DATA_PATH / "regulations" / (out["setup"]["s_name"].asString() + ".json"), out);
		}
		catch (exception & e)
		{
			logging.exceptionMessage(e, 5);
		}	}	try
	{
		PythonFunctions::configure("regulations");
	}
	catch (exception & e)
	{
		logging.exceptionMessage(e, 6);
	}

	return 0;
}