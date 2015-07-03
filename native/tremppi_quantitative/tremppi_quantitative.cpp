#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/general/program_options.hpp>
#include <tremppi_common/report/report.hpp>
#include <tremppi_common/python/python_functions.hpp>

struct ComputedData {
	string name;
	size_t count;
	double min;
	double max;
	double mean;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_qualitative
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tremppi_quantitative(int argc, char ** argv) {

	bpo::variables_map po = TremppiSystem::initiate<ProgramOptions>("tremppi_quantitative", argc, argv);
	Logging logging;

	Json::Value out;
	RegInfos reg_infos;
	sqlite3pp::database db;
	map<size_t, string> columns;
	const vector<string> prefixes = { "K", "C", "R", "E", "B", "I" };
	try {
		DEBUG_LOG << "Parsing data.";

		// Read filter conditions
		out = Report::createSetup();

		db = move(sqlite3pp::database((TremppiSystem::DATA_PATH / DATABASE_FILENAME).string().c_str()));

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

	vector<ComputedData> results;
	try {
		DEBUG_LOG << "Preparing the data.";
		for (const pair<size_t, string> column : columns) {
			results.push_back(ComputedData{ column.second, 0, numeric_limits<double>::max(), -1 * numeric_limits<double>::max(), 0 });
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	const size_t row_count = sqlite3pp::func::rowCount(PARAMETRIZATIONS_TABLE, out["setup"]["select"].asString(), db);
	try {

		DEBUG_LOG << "Reading the values, computing the statistics.";

		logging.newPhase("Reading row", row_count);

		sqlite3pp::query group_qry = DatabaseReader::selectionFilter(columns, out["setup"]["select"].asString(), db);

		// Read the data
		for (auto row : group_qry) {
			for (int i = 0; i < group_qry.column_count(); i++) {
				if (row.column_type(i) == SQLITE_NULL) {
					throw runtime_error(string("A null valued entry in the column ") + group_qry.column_name(i));
				}

				double val;
				if (row.column_type(i) == SQLITE_INTEGER) {
					val = row.get<int>(i);
				}
				else if (row.column_type(i) == SQLITE_FLOAT) {
					val = row.get<double>(i);
				}
				if (val != 0) {
					results[i].count++;
				}
				results[i].min = min(results[i].min, val);
				results[i].max = max(results[i].max, val);
				results[i].mean += val;

			}
			logging.step();

			// Compute mean
			for (ComputedData & result : results) {
				result.mean = result.mean / row_count;
			}
		}
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 3);
	}

	try {
		DEBUG_LOG << "Building the JSON file.";
		// For each graph create the graph data and add configuration details

		for (ComputedData & result : results) {
			Json::Value result_node;
			result_node["name"] = Report::reformName(result.name);
			result_node["count"] = static_cast<Json::Value::UInt>(result.count);
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
		DEBUG_LOG << "Writing output.";

		FileManipulation::writeJSON(TremppiSystem::DATA_PATH / "quantitative" / (out["setup"]["s_name"].asString() + ".json"), out);
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 5);
	}

	try {
		PythonFunctions::configure("quantitative");
	}
	catch (exception & e) {
		logging.exceptionMessage(e, 6);
	}

	return 0;
}
