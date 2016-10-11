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

#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/network/data_info.hpp>
#include <tremppi_common/network/definitions.hpp>
#include <json/json.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Checks the model for the correctness of the syntax, except for the constraints (those need kinetics to be built first)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace SyntaxChecker
{
	// Guarantees that the maximal values are integers and that they are convertible to int
	void checkMaxes(const Json::Value & nodes);

	// Guarantees that the IDs in nodes meet the requirements for an ID and are readable
	void checkIDs(const Json::Value & nodes);

	// Guarantees that the names in nodes meet the requirements for an ID and are readable
	void checkNames(const Json::Value & nodes);

	// Provides the id_to_name map
	map<string, string> idsToNames(const Json::Value & nodes);

	// Control one endpoint of an edge
	void checkEdgeEnd(const Json::Value & edge, const string & type, const map<string, ActLevel> & components);

	// Control if the edges have their ids in the component list
	void checkEdges(const Json::Value & edges, const map<string, ActLevel> & components);

	const inline string getEdgeName(const Json::Value & edge, const map<string, string> & ids_to_names);

	// Control if the thresholds are in the range of their source
	void checkThresholds(const Json::Value & edges, const map<string, ActLevel> & components, const map<string, string> & ids_to_names);

	// Checks if all the edge labels are correct
	void checkLabels(const Json::Value & edges, const map<string, string> & ids_to_names);

	// Control that the values are not missing and are of the right form
	void controlSemantics(const Json::Value & elements);
};