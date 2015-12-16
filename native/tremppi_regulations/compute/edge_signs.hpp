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

namespace EdgeSigns 
{
	// Divide the number of parametrizations in total by the number of parametrizations sharing the same regulatory function
	double computeExpectedFreq(const RegInfo & reg_info, const CompID reg_ID) 
	{
		const size_t context_c = accumulate(WHOLE(reg_info.regulators), 1, [](const size_t part_val, const pair<CompID, Levels> & regulator) {
			return part_val * (regulator.second.size() + 1);
		});
		const size_t other_c = context_c / (reg_info.regulators.at(reg_ID).size() + 1);
		double all_params = pow(reg_info.max_activity + 1, context_c);
		double same_params = pow(reg_info.max_activity + 1, other_c) * pow(reg_info.max_activity + 1, other_c * (reg_info.regulators.at(reg_ID).size() - 1));

		return 1.0 - (same_params / all_params);
	}

	void computeExpectedFreq(const RegInfo & reg_info, map<CompID, double> & result) 
	{
		for (const auto & regulator : reg_info.regulators) 
		{
			result[regulator.first] = computeExpectedFreq(reg_info, regulator.first);
		}
	}

	void compute(const RegInfos & reg_infos, const CompID ID, const int step_count, sqlite3pp::query & qry, map<CompID, vector<double> > & freq, map<CompID, vector<char> > & sign) 
	{
		map<Regulation, size_t> column_of_regulation;
		for (const auto & regulator : reg_infos[ID].regulators) 
		{
			for (ActLevel threshold : regulator.second) {
				string column_name = "S_" + reg_infos[regulator.first].name + "_" + to_string(threshold) + "_" + reg_infos[ID].name;
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
			for (const auto & regulator : reg_infos[ID].regulators) 
			{
				for (size_t threshold_i : cscope(regulator.second)) 
				{
					const ActLevel threshold = regulator.second[threshold_i];
					Regulation regulation = Regulation{ reg_infos[regulator.first].ID, threshold, reg_infos[ID].ID };
					size_t column_i = column_of_regulation[regulation];
					char label = row.get<const char *>(column_i)[0];
					if (label != '0') 
					{
						freq[regulator.first][threshold_i]++;
						sign[regulator.first][threshold_i] |= label;
					}
				}
			}
		}

		for (auto & freq_pair : freq)
		{
			Statistics::normalize(step_count, freq_pair.second);
		}

		for (const auto & regulator : reg_infos[ID].regulators) 
		{
			for (size_t threshold_i : cscope(regulator.second)) 
			{
				// no active edge at all
				if (sign[regulator.first][threshold_i] == 0) 
				{
					sign[regulator.first][threshold_i] = '0';
				}
				// there were active edges and they were not either only + or only -
				else if (sign[regulator.first][threshold_i] != '+' && sign[regulator.first][threshold_i] != '-') 
				{
					sign[regulator.first][threshold_i] = '1';
				}
			}
		}
	}
}