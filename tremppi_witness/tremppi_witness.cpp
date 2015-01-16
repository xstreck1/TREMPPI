#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/report/report.hpp>

#include "io/witness_options.hpp"
#include "io/witness_reader.hpp"
#include "io/witness_output.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_validate.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_witness(int argc, char ** argv) {
	bpo::variables_map po = TremppiSystem::initiate<WitnessOptions>("tremppi_witness", argc, argv);
	Logging logging;

	Json::Value out;
	Json::Value properties;
	RegInfos reg_infos;
	sqlite3pp::database db;
	// Obtain data
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing data.";

		// Create setup
		out = Report::createSetup();

		// Get properties 
		properties = FileManipulation::parseJSON(TremppiSystem::WORK_PATH / PROPERTIES_FILENAME);

		// Copy the data
		FileManipulation::copyAnalysisFiles(TremppiSystem::WORK_PATH / ("witness_" + TimeManager::getTimeStamp()), "witness");

		// Get database
		db = move(sqlite3pp::database((TremppiSystem::WORK_PATH / DATABASE_FILENAME).string().c_str()));

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
		for (const string & comp_name : DataInfo::getAllNames(reg_infos))
			out["setup"]["components"].append(comp_name);

	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}


	// Obtain the nodes and write to the the set
	set<pair<string, string>> transitions;
	WitnessReader wit_reader;
	try {
		BOOST_LOG_TRIVIAL(info) << "Loading witnesses.";
		logging.newPhase("computing witness", properties.size());

		for (const auto & property : properties) {
			// If selected
			if (property["desc"][0]["values"]["Verify"].asBool()) {
				const string name = property["desc"][0]["values"]["Name"].asString();
				out["setup"]["properties"] = out["setup"]["properties"].asString() + name + ",";

				wit_reader.select(name, out["setup"]["select"].asString(), db);
				// Read transitions
				while (wit_reader.next()) {
					set<pair<string, string>> new_transitions = wit_reader.getWitness();
					transitions.insert(WHOLE(new_transitions));
				}
			}
			logging.step();
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Converting to JSON";

		out["elements"] = WitnessOutput::convert(transitions);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}

	// Output 
	try {
		BOOST_LOG_TRIVIAL(info) << "Writing output.";
		// Write the computed content
		Json::StyledWriter writer;
        bfs::path output_path = TremppiSystem::WORK_PATH / ( "witness_" + TimeManager::getTimeStamp() + ".json");
        fstream data_file(output_path.string(), ios::out);
        if (!data_file)
            throw runtime_error("Could not open " + output_path.string());
		string data = writer.write(out);
		data_file  << data << endl;
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 5);
	}

	return 0;
}
