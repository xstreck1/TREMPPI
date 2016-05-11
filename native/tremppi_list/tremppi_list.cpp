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
int tremppi_list(int argc, char ** argv)
{
    TremppiSystem::initiate("tremppi_list", argc, argv);
    Logging logging;

    RegInfos reg_infos;
    sqlite3pp::database db;
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
