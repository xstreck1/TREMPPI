#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/time_manager.hpp>

#include "io/witness_options.hpp"
#include "io/witness_reader.hpp"
#include "io/witness_output.hpp"

// TODO: Check if the transition generation is correct for all three components (w.r.t. the transition system used)
// TODO: Components still required to be ordered...

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_validate.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_witness(int argc, char ** argv) {
	bpo::variables_map po = tremppi_system.initiate<WitnessOptions>("tremppi_witness", argc, argv);
	string select;

	Json::Value out;
	out["setup"]["date"] = TimeManager::getTime();
	string time_stamp = TimeManager::getTimeStamp();
	bfs::path witness_path = tremppi_system.WORK_PATH / ("witness_" + time_stamp);
	RegInfos reg_infos;
	sqlite3pp::database db;
	map<size_t, string> prop_columns;
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing database.";

		// Get selection		
		select = DatabaseReader::getSelectionTerm("Select");

		// Copy the data
		WitnessOutput::copyWitnessFiles(witness_path);

		// Get database
		db = move(sqlite3pp::database((tremppi_system.WORK_PATH / DATABASE_FILENAME).string().c_str()));

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
		prop_columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex("W_.*"), db);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	// Obtain the nodes and write to the JSON
	try {
		for (const auto & prop_column : prop_columns) {
			WitnessReader wit_reader;
			wit_reader.select(prop_column.second, select, db);
			set<pair<string, string>> transitions;
			
			// Read transitions
			while (wit_reader.next()) {
				set<pair<string, string>> new_transitions = wit_reader.getWitness();
				transitions.insert(WHOLE(new_transitions));
			}

			if (transitions.empty())
				continue;

			Json::Value & elements = out[prop_column.second]["elements"];
			// Add transitions
			Json::Value & edges = elements["edges"];
			set<string> states;
			for (const pair<string, string> & transition : transitions) {
				Json::Value edge;
				edge["data"]["id"] = to_string(elements["edges"].size());
				edge["data"]["source"] = transition.first;
				edge["data"]["target"] = transition.second;
				edges.append(edge);
				states.insert(transition.first);
				states.insert(transition.second);
			}

			// Add nodes
			Json::Value & nodes = elements["nodes"];
			for (const string & state : states) {
				Json::Value node;
				node["data"]["id"] = state;
				nodes.append(node);
			}
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 6);
	}

	// Output 
	try {
		BOOST_LOG_TRIVIAL(info) << "Writing output.";
		// Write the computed content
		Json::StyledWriter writer;
        bfs::path output_path = tremppi_system.WORK_PATH / ( "witness_" + time_stamp + ".js");
        fstream data_file(output_path.string(), ios::out);
        if (!data_file)
            throw runtime_error("Could not open " + output_path.string());
		string data = writer.write(out);
		data_file << "var witness = " << data << ";" << endl;
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 7);
	}

	return 0;
}
