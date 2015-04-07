#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/general/program_options.hpp>
#include <tremppi_common/report/report.hpp>

struct Computed {
	string name;
	size_t count;
	double portion;
	double min;
	double max;
	double mean;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_summary
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_summary(int argc, char ** argv) {

	bpo::variables_map po = TremppiSystem::initiate<ProgramOptions>("tremppi_summary", argc, argv);
	Logging logging;

	Json::Value out;
	Json::Value summary;
	RegInfos reg_infos;
	sqlite3pp::database db;
	map<size_t, string> columns;
	const vector<string> prefixes = { "K", "C", "R" };
	try {
		BOOST_LOG_TRIVIAL(info) << "Parsing data.";

		// Read filter conditions
		out = Report::createSetup();

		summary = FileManipulation::parseJSON(TremppiSystem::WORK_PATH / SUMMARY_FILENAME);

		db = move(sqlite3pp::database((TremppiSystem::WORK_PATH / DATABASE_FILENAME).string().c_str()));

		// Read regulatory information
		DatabaseReader reader;
		reg_infos = reader.readRegInfos(db);

		for (const string & prefix : prefixes) {
			const auto new_columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex{ prefix + "_.*" }, db);
			columns.insert(WHOLE(new_columns));
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 2);
	}

	vector<Computed> results;
	try {
		BOOST_LOG_TRIVIAL(info) << "Preparing the data.";
		for (const pair<size_t, string> column : columns) {
			results.emplace_back(Computed{ column.second, 0, 0, 0, 0, 0 });
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	const size_t row_count = sqlite3pp::func::rowCount(PARAMETRIZATIONS_TABLE, out["setup"]["select"].asString(), db);
	try {

		BOOST_LOG_TRIVIAL(info) << "Reading the values, computing the statistics.";

		logging.newPhase("Harvesting component", row_count);

		sqlite3pp::query group_qry = DatabaseReader::selectionFilter(columns, out["setup"]["select"].asString(), db);

		// Read the data
		for (auto row : group_qry) {
			for (int i = 0; i < group_qry.column_count(); i++) {
				if (row.column_type(i) != SQLITE_NULL) {
					double val;
					if (row.column_type(i) == SQLITE_INTEGER) {
						val = row.get<int>(i);
					} else if (row.column_type(i) == SQLITE_FLOAT) {
						val = row.get<double>(i);
					}
					results[i].count++;
					results[i].min = min(results[i].min, val);
					results[i].max = max(results[i].max, val);
					results[i].mean += val;
				}
			}
			logging.step();
		}

		// Compute remaning values
		for (Computed & result : results) {
			result.portion = static_cast<double>(result.count) / row_count;
			if (result.count > 0)
				result.mean /= static_cast<double>(result.count);
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Building the JSON file.";
		// For each graph create the graph data and add configuration details

		for (Computed & result : results) {
			Json::Value result_node;
			result_node["name"] = result.name;
			result_node["count"] = result.count;
			result_node["portion"] = result.portion;
			result_node["min"] = result.min;
			result_node["max"] = result.max;
			result_node["mean"] = result.mean;
			out["records"].append(result_node);
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 4);
	}

	try {
		BOOST_LOG_TRIVIAL(info) << "Writing output.";
		Json::Value new_summary;
		new_summary["id"] = TimeManager::getTimeStamp();
		new_summary["text"] = TimeManager::getTimeStamp();
		summary["selections"].append(new_summary);
		FileManipulation::writeJSON(TremppiSystem::WORK_PATH / "summary.json", summary);
		FileManipulation::writeJSON(TremppiSystem::WORK_PATH / "summary" / (TimeManager::getTimeStamp() + ".json"), out);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 5);
	}

	return 0;
}
