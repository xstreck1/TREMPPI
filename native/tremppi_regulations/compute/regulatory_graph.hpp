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

#pragma once

#include <tremppi_common/database/database_reader.hpp>
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/report/statistics.hpp>

#include "../data/data_storage.hpp"
namespace RegulatoryGraph 
{
	// computes the correlation of the regulators to the regulated component for each component
	void compute(const RegInfos & reg_infos, const CompID ID, const int step_count, sqlite3pp::query & qry, map<CompID, vector<double> > & corr) 
	{
		map<Regulation, size_t> column_of_regulation;
		for (const auto & regulator : reg_infos[ID].regulators) 
		{
			for (ActLevel threshold : regulator.second) {
				string column_name = "I_" + reg_infos[regulator.first].name + "_" + to_string(threshold) + "_" + reg_infos[ID].name;
				Regulation regulation = Regulation{ reg_infos[regulator.first].ID, threshold, reg_infos[ID].ID };
				for (const size_t column_i : crange(qry.column_count())) 
				{
					string database_column = qry.column_name(column_i);
					if (database_column == column_name) 
					{
						column_of_regulation[regulation] = column_i;
					}
				}
				if (column_of_regulation.count(regulation) == 0) 
				{
					throw runtime_error("did not find the regulation " + column_name + " in the database");
				}
			}
		}

		// Compute relation to each regulation of this component, step by database rows
		for (const auto & row : qry) 
		{
			for (const auto & regulator : reg_infos[ID].regulators) 			{
				for (size_t threshold_i : cscope(regulator.second)) 
				{
					const ActLevel threshold = regulator.second[threshold_i];
					Regulation regulation = Regulation{ reg_infos[regulator.first].ID, threshold, reg_infos[ID].ID };
					size_t column_i = column_of_regulation[regulation];
					double impact = row.get<double>(column_i);
					corr[regulator.first][threshold_i] += impact;
				}
			}
		}
		
		for (auto & corr_pair : corr) 
		{
			Statistics::normalize(step_count, corr_pair.second);
		}
	}
}