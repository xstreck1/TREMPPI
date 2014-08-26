/*
 * Copyright (C) 2012-2013 - Adam Streck
 * This file is a part of the ParSyBoNe (Parameter Synthetizer for Boolean Networks) verification tool.
 * ParSyBoNe is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.
 * ParSyBoNe is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>.
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> and <http://sybila.fi.muni.cz/>.
 */

#ifndef USAGE_HPP
#define USAGE_HPP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file This file holds version-specific data.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tremppi_common/network/definitions.hpp>

struct Version {
   size_t major;
   size_t minor;
   enum Stage {alpha, beta, rc, release} stage;
   size_t stage_number;

   friend ostream& operator<< (ostream &out, const Version &ver) {
      out << ver.major << "." << ver.minor << ".";
      switch (ver.stage) {
      case alpha:
         out << "alpha";
         break;
      case beta:
         out << "beta";
         break;
      case rc:
         out << "rc";
         break;
      case release:
         out << "release";
         break;
      }
      out << "." << ver.stage_number;
      return out;
   }
};

const Version getVersion() {
   return {2, 2, Version::alpha, 0};
}

const string getUsage() {
   return
         "parsybone model.pmf property.ppf [database1.sqlite,...] [-cdfmrvwW] [--bound N] [--data database_file] [--file text_file] [--dist I N] [--help] [--ver]\n"
         "\n"
         "model.pmf            name of the file that will be parsed and used, must have a .pmf suffix; model is used as the name of the model (and thus impicit output) further in the program\n"
         "property.ppf         name of the property file that will be parset and used with the model, must have a .ppf suffix\n"
         "database1.sqlite,... a list of database files that are used for filtering the parametrizations; if provided only those parametrizations that are present in all the databases are considered for the computation\n"
         "\n"
         "-c display computation results on console\n"
         "-d output computation results to an sqlite database\n"
         "   by default the file is model.sqlite, can be changed using the --data switch\n"
         "-f output computation results to a text file\n"
         "   by default the file is model.out, can be changed using the --file switch\n"
         "-m minimize the Cost - only parametrizations whose paths have globaly minimal cost will be output\n"
         "-n negate the property - only the parametrizations that do not satisfy the property will be output\n"
         "-r compute robustness of a time series\n"
         "-v verbose (output progress to console)\n"
         "-w compute witnesses and store them in encoded form\n"
         "-W compute witnesses and store them in explicit form\n"
         "\n"
         "--bound constraints the depth of a depth-first search to the value N\n"
         "--dist  used for distributed computation with two integers, denoting the I-th process out of N. Total - each of those tests only 1/N of the parametrization space.\n"
         "--help  display help\n"
         "--ver   display the current version\n"
         ;
}

#endif // USAGE_HPP
