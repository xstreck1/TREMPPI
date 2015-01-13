#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/time_manager.hpp>

#include "io/witness_options.hpp"
#include "io/witness_reader.hpp"
#include "io/witness_output.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_validate.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_witness(int argc, char ** argv) {
	bpo::variables_map po = tremppi_system.initiate<WitnessOptions>("tremppi_witness", argc, argv);
	Logging logging;
	string select;

	Json::Value out;
	Json::Value properties;
	out["setup"]["date"] = TimeManager::getTime();
	string time_stamp = TimeManager::getTimeStamp();
	RegInfos reg_infos;
	sqlite3pp::database db;
	// Obtain data
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing data.";

		// Get selection		
		select = DatabaseReader::getSelectionTerm("Select");

		// Get properties 
		properties = FileManipulation::parseJSON(tremppi_system.WORK_PATH / PROPERTIES_FILENAME);

		// Copy the data
		FileManipulation::copyAnalysisFiles(tremppi_system.WORK_PATH / ("witness_" + time_stamp), "witness");

		// Get database
		db = move(sqlite3pp::database((tremppi_system.WORK_PATH / DATABASE_FILENAME).string().c_str()));

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);
		for (const string & comp_name : DataInfo::getAllNames(reg_infos))
			out["setup"]["components"].append(comp_name);

	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}


	// Obtain the nodes and write to the JSON
	try {
		BOOST_LOG_TRIVIAL(info) << "Storing witnesses.";
		logging.newPhase("computing witness", properties.size());

		for (const auto & property : properties) {
			// If selected
			if (property["desc"][0]["values"]["Verify"].asBool()) {
				const string name = property["desc"][0]["values"]["Name"].asString();
				Json::Value witness = WitnessOutput::addPropert(name, select, db);
				// If there is even a witness
				if (!witness.empty()) {
					out[name]["elements"] = witness;
				}
			}
			logging.step();
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	// Output 
	try {
		BOOST_LOG_TRIVIAL(info) << "Writing output.";
		// Write the computed content
		Json::StyledWriter writer;
        bfs::path output_path = tremppi_system.WORK_PATH / ( "witness_" + time_stamp + ".json");
        fstream data_file(output_path.string(), ios::out);
        if (!data_file)
            throw runtime_error("Could not open " + output_path.string());
		string data = writer.write(out);
		data_file  << data << endl;
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}

	return 0;
}
