/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <tremppi_common/general/system.hpp>
#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/time_manager.hpp>
#include <tremppi_common/report/report.hpp>
#include <tremppi_common/python/python_functions.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Entry point of tremppi_list
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ComputedQual
{
    string name;
    map<string, size_t> values;
};

inline void incrementOccurence(const string & value, map<string, size_t> & values)
{
    if (values.count(value) == 0) {
        values.insert(make_pair(value, 1));
    }
    else
    {
        values[value]++;
    }
}

int tremppi_list(int argc, char ** argv)
{
    TremppiSystem::initiate("tremppi_list", argc, argv);
    Logging logging;
    const vector<string> prefixes = { "S", "C" };

    RegInfos reg_infos;
    sqlite3pp::database db;
    DatabaseReader reader;

    vector<string> selections;
    vector<string> sels_name;
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
    logging.newPhase("making report", selections.size());
    for (const size_t sel_no : cscope(selections))
    {
        Json::Value out;
        map<size_t, string> columns;
        vector<ComputedQual> results;
        try
        {
            DEBUG_LOG << "Selection " + sels_name[sel_no];
            out = Report::createSetup(selections[sel_no], sels_name[sel_no]);
            // Read filter conditions
            out = Report::createSetup(selections[sel_no], sels_name[sel_no]);

            // Filter for all S and C prefixes
            for (const string & prefix : prefixes)
            {
//               if(prefix == "S" )
//                {
                const auto new_columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex{ prefix + "_.*" }, db);

                columns.insert(WHOLE(new_columns));
//                // use column names to iterate through columns, ask if exists 0 in column
//                    for (int i = 0; i < columns.size(); i++)
//                    {
//                       const size_t optional= sqlite3pp::query(db, ("SELECT CAST(CASE WHEN COUNT(*) > 0 THEN 1 ELSE 0 END AS BIT)" + " FROM " + PARAMETRIZATIONS_TABLE + " WHERE " + column.second(i) "=0").c_str());
//                       // SELECT CAST(CASE WHEN COUNT(*) > 0 THEN 1 ELSE 0 END AS BIT) FROM Parametrizations WHERE = 0;
//                       // returns 1 if there exists a 0 in column

//                    }
//                }
//                else
//                {
//                const auto new_columns = sqlite3pp::func::matchingColumns(PARAMETRIZATIONS_TABLE, regex{ prefix + "_.*" }, db);
//                columns.insert(WHOLE(new_columns));
//                }
//            }
            for (const pair<size_t, string> column : columns)
            {
                results.emplace_back(ComputedQual{ column.second, { } });
            }

        }
        catch (exception & e)
        {
            logging.exceptionMessage(e, 3);
        }

        // add non optional egdes to the selection filter

        const size_t row_count = sqlite3pp::func::rowCount(PARAMETRIZATIONS_TABLE, out["setup"]["select"].asString(), db);
        const double row_count_d = row_count;
        try
        {
            DEBUG_LOG << "Reading the values, computing the statistics.";
            logging.newPhase("Reading row", row_count);
            sqlite3pp::query group_qry = DatabaseReader::selectionFilter(columns, out["setup"]["select"].asString(), db);

            // Read the data
            //string columns_list = alg::join(DataConv::columns2list(columns), ", ");
            //sqlite3pp::query(db, ("SELECT COUNT(*)," + columns_list + " FROM " + PARAMETRIZATIONS_TABLE + " WHERE " + selection).c_str() "GROUP BY" + columns_list);

            // go through table columnwise build vector for each column

            for (auto col : group_qry.column_count())
            {
                int i=0;
                for (auto row : group_qry)
                {
                    i++;
                    if (row.column_type(i) == SQLITE_NULL)
                    {
                        throw runtime_error(string("A null valued entry in the column ") + group_qry.column_name(i));
                    }
                    else if (row.column_type(i) == SQLITE_INTEGER)
                    {
                        incrementOccurence(to_string(row.get<int>(i)), results[i].values);
                    }
                    else if (row.column_type(i) == SQLITE_FLOAT)
                    {
                        incrementOccurence(to_string(row.get<double>(i)), results[i].values);
                    }
                    else if (row.column_type(i) == SQLITE3_TEXT)
                    {
                        incrementOccurence(row.get<const char *>(i), results[i].values);
                    }
                    else
                    {
                        throw runtime_error("Unknown SQLITE3 columne type " + to_string(SQLITE3_TEXT));
                    }
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
            DEBUG_LOG << "Writing the result to file.";
            // For each graph create the graph data and add configuration details
            for (ComputedQual & result : results)
            {
                Json::Value result_node;
                result_node["name"] = Report::reformName(reg_infos, result.name);
                for (const pair<string, size_t> & value : result.values)
                {
                    Json::Value data;
                    data["name"] = value.first;
                    data["portion"] = value.second / row_count_d;
                    result_node["data"].append(data);
                }
                out["values"].append(result_node);
            }

            FileManipulation::writeJSON(TremppiSystem::DATA_PATH / "list" / (out["setup"]["s_name"].asString() + ".json"), out);
        }
        catch (exception & e)
        {
            logging.exceptionMessage(e, 5);
        }	}
    try
    {
        PythonFunctions::configure("list");
    }
    catch (exception & e)
    {
        logging.exceptionMessage(e, 6);
    }

    return 0;
}
