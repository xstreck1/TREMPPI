#include <json/json.h>
#include <tpi_common/database_reader.hpp>
#include <tpi_common/file_manipulation.hpp>
#include <tpi_common/logging.hpp>
#include <tpi_common/sqlite3pp_func.hpp>
#include <tpi_common/time_manager.hpp>
#include "analysis/statistical_analysis.hpp"
#include "analysis/lattice_builder.hpp"
#include "analysis/regulatory_graph.hpp"
#include "io/data_reader.hpp"
#include "io/output.hpp"
#include "io/program_options.hpp"

// 
void exceptionMessage(const exception & e, const int err_no) {
	BOOST_LOG_TRIVIAL(error) << "Top level exception: " << e.what();
	exit(err_no);
}

// TODO: disable regulatory if not -r
int main(int argc, char ** argv) {
	Logging::init(PROGRAM_NAME + ".log");
	BOOST_LOG_TRIVIAL(info) << "TREMPPI Statistical Analysis of Parametrization Space (" << PROGRAM_NAME << ") started.";

	map<string, Json::Value> out;
	out["setup"]["date"] = TimeManager::getTime();

	bpo::variables_map po;
	bfs::path input_path;

	vector<RegInfo> source_data;
	database db;

	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing data file.";
		// Get user options
		po = ProgramOptions::parseProgramOptions(argc, argv);
		out["setup"]["comparative"] = po.count("compare") > 0;
		input_path = ProgramOptions::getDatabasePath(po);

		// Copy the directory data
		Output::copyReport(input_path);

		// Get database
		out["setup"]["name"] = input_path.stem().string();
		db = move(database(po["database"].as<string>().c_str()));

		// Read filter conditions
		if (po.count("select") > 0)
			out["setup"]["select"] = ProgramOptions::getFilter(po["select"].as<string>());
		if (po.count("compare") > 0)
			out["setup"]["compare"] = ProgramOptions::getFilter(po["compare"].as<string>());

		// Read regulatory information
		query qry(db, ("SELECT " + DatabaseReader::NAMES_COLUMN + " FROM " + COMPONENTS_TABLE).c_str());
		for (auto row : qry) {
			string name = row.get<const char*>(0);
			source_data.push_back(DatabaseReader::readRegInfo(name, db));
		}
	}
	catch (exception & e) {
		exceptionMessage(e, 1);
	}

	map<string, RegsData> reg_data_types;
	map<string, FunsData> fun_data_types;
	try {
		out["setup"]["pool_size"] = begin(query(db, ("SELECT COUNT(*) FROM " + PARAMETRIZATIONS_TABLE).c_str()))->get<int>(0);
		if (out["setup"]["select"].asString().empty())
			out["setup"]["selected"] = out["setup"]["pool_size"];
		else
			out["setup"]["selected"] = begin(query(db, ("SELECT COUNT(*) FROM " + PARAMETRIZATIONS_TABLE + " WHERE " + out["setup"]["select"].asString()).c_str()))->get<int>(0);
		if (out["setup"]["compare"].asString().empty())
			out["setup"]["compared"] = out["setup"]["pool_size"];
		else
			out["setup"]["compared"] = begin(query(db, ("SELECT COUNT(*) FROM " + PARAMETRIZATIONS_TABLE + " WHERE " + out["setup"]["compare"].asString()).c_str()))->get<int>(0);

		if (po.count("functions") > 0) {
			BOOST_LOG_TRIVIAL(info) << "Computing regulatory functions data.";
			fun_data_types["select"] = FunsData(); 
			if (po.count("compare") > 0) {
				fun_data_types["compare"] = FunsData();
				fun_data_types["differ"] = FunsData();
			}
			// Get function statistics for all the regulators
			for (const RegInfo & reg_info : source_data) {
				query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["select"].asString(), db);
				fun_data_types["select"].emplace_back(StatisticalAnalysis::build(reg_info, out["setup"]["selected"].asInt(), sel_qry));
				// Get the statistics for the compare selection
				if (po.count("compare") > 0) {
					query com_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["compare"].asString(), db);
					fun_data_types["compare"].emplace_back(StatisticalAnalysis::build(reg_info, out["setup"]["compared"].asInt(), com_qry));
				}
			}
			// Compute difference
			if (po.count("compare") > 0)
				fun_data_types["differ"] = move(StatisticalAnalysis::diff(fun_data_types["select"], fun_data_types["compare"]));
		}

		if (po.count("regulations") > 0) {
			BOOST_LOG_TRIVIAL(info) << "Computing regulatory graph data.";
			reg_data_types["select"] = RegsData(); 
			if (po.count("compare") > 0) {
				reg_data_types["compare"] = RegsData(); 
				reg_data_types["differ"] = RegsData();
			}
			for (const RegInfo & reg_info : source_data) {
				query sel_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["select"].asString(), db);
				reg_data_types["select"].emplace_back(RegulatoryGraph::build(reg_info, out["setup"]["selected"].asInt(), sel_qry));
				// Get the statistics for the compare selection
				if (po.count("compare") > 0) {
					query com_qry = DatabaseReader::selectionFilter(reg_info.columns, out["setup"]["compare"].asString(), db);
					reg_data_types["compare"].emplace_back(RegulatoryGraph::build(reg_info, out["setup"]["compared"].asInt(), com_qry));
				}
			}
			// Compute difference
			if (po.count("compare") > 0)
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
		exceptionMessage(e, 2);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Building the JSON files.";

		// For each graph create the graph data and add configuration details
		for (auto & regs : reg_data_types) {
			out[string("Regulatory")][regs.first] = Output::regulatoryGraph(regs);
		}
		for (auto & funs : fun_data_types) {
			out[string("Functional")][funs.first] = Output::functionalData(funs);
		}
	}
	catch (exception & e) {
		exceptionMessage(e, 3);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Writing output.";
		// Write the computed content
		Json::StyledWriter writer;
		ofstream data_file = Output::fileOutput(input_path, "data.js");
		for (const auto & file_data : out) {
			ofstream own_file = Output::fileOutput(input_path, file_data.first + ".json");
			string data = writer.write(file_data.second);
			own_file << data;
			data_file << PROGRAM_NAME << "." << file_data.first << " = " << data << ";" << endl;
		}
	}
	catch (exception & e) {
		exceptionMessage(e, 4);
	}


	BOOST_LOG_TRIVIAL(info) << PROGRAM_NAME << " finished succesfully.";
	return 0;
}
