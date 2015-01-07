#include <json/json.h>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include "analysis/statistical_analysis.hpp"
#include "analysis/lattice_builder.hpp"
#include "analysis/regulatory_graph.hpp"
#include "io/data_reader.hpp"
#include "io/output.hpp"
#include "io/report_options.hpp"

// TODO: disable regulatory if not -r
int tremppi_report(int argc, char ** argv) {
	bpo::variables_map po = tremppi_system.initiate<ReportOptions>("tremppi_report", argc, argv);

	Json::Value out;
	out["setup"]["date"] = TimeManager::getTime();
	string time_stamp = TimeManager::getTimeStamp();
	bfs::path report_path = tremppi_system.WORK_PATH / ("report_" + time_stamp);
	RegInfos reg_infos;
	sqlite3pp::database db;
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing data file.";
		// Read filter conditions
		out["setup"]["select"] = DatabaseReader::getSelectionTerm("Select");
		out["setup"]["compare"] = DatabaseReader::getSelectionTerm("Compare");
		out["setup"]["comparative"] = out["setup"]["select"].asString() == out["setup"]["compare"].asString();

		// Copy the directory data
		Output::copyReport(report_path);

		// Get database
		out["setup"]["name"] = tremppi_system.WORK_PATH.stem().string();
		db = move(sqlite3pp::database((tremppi_system.WORK_PATH / DATABASE_FILENAME).string().c_str()));

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	map<string, RegsData> reg_data_types;
	map<string, FunsData> fun_data_types;
	try {
        out["setup"]["pool_size"] = (sqlite3pp::query(db, ("SELECT COUNT(*) FROM " + PARAMETRIZATIONS_TABLE).c_str()).begin())->get<int>(0);
        out["setup"]["selected"] = (sqlite3pp::query(db, ("SELECT COUNT(*) FROM " + PARAMETRIZATIONS_TABLE + " WHERE " + out["setup"]["select"].asString()).c_str()).begin())->get<int>(0);
		if (out["setup"]["comparative"].asBool())
            out["setup"]["compared"] = (sqlite3pp::query(db, ("SELECT COUNT(*) FROM " + PARAMETRIZATIONS_TABLE + " WHERE " + out["setup"]["compare"].asString()).c_str()).begin())->get<int>(0);
		else
			out["setup"]["compared"] = out["setup"]["pool_size"];

		/*if (po.count("functions") > 0) {
			BOOST_LOG_TRIVIAL(info) << "Computing regulatory functions data.";
			fun_data_types["select"] = FunsData(); 
			if (po.count("select-only") == 0) {
				fun_data_types["compare"] = FunsData();
				fun_data_types["differ"] = FunsData();
			}
			// Get function statistics for all the regulators
			for (const RegInfo & reg_info : reg_infos) {
				sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["select"].asString(), db);
				fun_data_types["select"].emplace_back(StatisticalAnalysis::build(reg_info, out["setup"]["selected"].asInt(), sel_qry));
				// Get the statistics for the compare selection
				if (po.count("select-only") == 0){
					sqlite3pp::query com_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["compare"].asString(), db);
					fun_data_types["compare"].emplace_back(StatisticalAnalysis::build(reg_info, out["setup"]["compared"].asInt(), com_qry));
				}
			}
			// Compute difference
			if (po.count("select-only") == 0)
				fun_data_types["differ"] = move(StatisticalAnalysis::diff(fun_data_types["select"], fun_data_types["compare"]));
		}*/

		if (po.count("regulations") > 0) {
			BOOST_LOG_TRIVIAL(info) << "Computing regulatory graph data.";
			reg_data_types["select"] = RegsData(); 
			if (out["setup"]["comparative"].asBool()) {
				reg_data_types["compare"] = RegsData(); 
				reg_data_types["differ"] = RegsData();
			}
			for (const RegInfo & reg_info : reg_infos) {
				sqlite3pp::query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["select"].asString(), db);
				reg_data_types["select"].emplace_back(RegulatoryGraph::build(reg_info, out["setup"]["selected"].asInt(), sel_qry));
				// Get the statistics for the compare selection
				if (out["setup"]["comparative"].asBool()) {
					sqlite3pp::query com_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["compare"].asString(), db);
					reg_data_types["compare"].emplace_back(RegulatoryGraph::build(reg_info, out["setup"]["compared"].asInt(), com_qry));
				}
			}
			// Compute difference
			if (out["setup"]["comparative"].asBool())
				reg_data_types["differ"] = move(RegulatoryGraph::diff(reg_data_types["select"], reg_data_types["compare"]));
		}

		/*if (po.count("graph") > 0) {
		BOOST_LOG_TRIVIAL(info) << "Computing parametrization graph data.";
		LatticeBuilder::makeParLatt(sel_par);
		if (po.count("compare") > 0) {
		LatticeBuilder::makeParLatt(cmp_par);
		}
		}*/
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Building the JSON files.";

		// For each graph create the graph data and add configuration details
		for (auto & regs : reg_data_types) {
			out["Regulatory"][regs.first] = Output::regulatoryGraph(reg_infos, regs);
		}
		/*for (auto & funs : fun_data_types) {
			out["Functional"][funs.first] = Output::functionalData(funs);
		}*/
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Writing output.";
		// Write the computed content
        Json::StyledWriter writer;

        bfs::path output_path = tremppi_system.WORK_PATH / ( "report_" + time_stamp + ".js");
        fstream data_file(output_path.string(), ios::out);
        if (!data_file)
            throw runtime_error("Could not open " + output_path.string());

		string data = writer.write(out);
		data_file << "var report = " << data << ";" << endl;
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 5);
	}

	BOOST_LOG_TRIVIAL(info) << tremppi_system.PROGRAM_NAME << " finished succesfully.";
	return 0;
}
