/*
 * Copyright (C) 2012-2013 - Adam Streck
 * This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
 * ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
 * ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
 */

#ifndef PARSYBONE_USER_OPTIONS_INCLUDED
#define PARSYBONE_USER_OPTIONS_INCLUDED

#include <tremppi_common/network/definitions.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Storage of options obtained from execution arguments.
///
/// Class that stores options provided by the user on the input. Values can be set up only using the ArgumentParser object.
/// Further access to global object user_options is possible only due to constant getters.
/// Only a single object is intended to exist.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UserOptions {
   bool be_verbose; ///< Display data from verbose stream (verbose_str), mainly progress information
   bool compute_robustness; ///< Should robustness value be computed and displyed?
   bool compute_wintess; ///< Should witnesses be computed and displayed?
   bool use_long_witnesses; ///< How witnesses should be displayed - complete state or only ID?
   bool output_console;
   bool use_textfile;
   bool use_database;
   bool minimalize_cost;
   bool produce_negative; ///< If true, only those for whom a witness (here a counter-example) is not found.
   size_t bound_size;
   size_t process_number; ///< What is the ID of this process?
   size_t processes_count; ///< How many processes are included in the computation?
   string model_path;
   string property_path;
   string model_name; ///< What is the name of the model?
   string property_name; ///< Name of the property employed.
   string database_file;
   string datatext_file;
   vector<string> filter_databases;

   /**
    * @brief addDefaultFiles    Create default file names where there are none explicitly specified.
    */
   void addDefaultFiles() {
      if (database_file.empty())
         database_file = model_path + model_name + "_" + property_name + DATABASE_SUFFIX;
      if (datatext_file.empty())
         datatext_file = model_path + model_name + "_" + property_name + OUTPUT_SUFFIX;
      if (find(filter_databases.begin(), filter_databases.end(), database_file) != filter_databases.end())
         throw invalid_argument(database_file + " is reserved for an output database and cannot be used as a filter.");
   }

   /**
    * Constructor, sets up default values.
    */
   UserOptions() {
      compute_wintess = minimalize_cost = be_verbose = use_long_witnesses = compute_robustness = output_console = use_textfile = use_database = produce_negative = false;
      database_file = datatext_file = "";
      bound_size = INF;
      process_number = processes_count = 1;
      model_path = model_name = "";
   }

   inline bool analysis() const {
      return (compute_robustness | compute_wintess);
   }

}; ///< Single program-shared user options object.

#endif // PARSYBONE_USER_OPTIONS_INCLUDED
