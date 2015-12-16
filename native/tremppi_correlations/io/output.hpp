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
#include "../data/data_storage.hpp"

namespace Output 
{
	Json::Value functionalData(const FunsData & funs_data) {
		Json::Value elements;

		// Write the nodes
		string nodes;
		elements["nodes"].resize(0);
		for (const FunData & fun : funs_data) 
		{
			Json::Value node, data;
			data["id"] = fun.info.name;
			data["name"] = fun.info.name;
			data["Bias"] = fun.mean;
			data["Mean"] = fun.mean;
			data["Std"] = fun.std_dev;
			data["Max"] = fun.info.max_activity;

			node["data"] = data;
			elements["nodes"].append(node);
		}

		// Write the edges
		elements["edges"].resize(0);
		for (const FunData & fun : funs_data) 
		{
			size_t reg_i = 0;
			for (const int i : cscope(fun.corrs)) 
			{
				Json::Value edge, data;

				data["source"] = fun.info.name;
				data["target"] = funs_data[i].info.name;
				data["Pearson"] = fun.corrs[i]; 
				
				edge["data"] = data;
				elements["edges"].append(edge);
			}
		}
		return elements;
	}
}