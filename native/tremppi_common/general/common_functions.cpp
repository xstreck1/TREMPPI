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

#include "common_functions.hpp"

std::vector<std::string> Common::getAllMatches(const std::string & control_regex, const std::string & original, const int n) 
{
	std::vector<std::string> result;

	std::string source = original;
	std::smatch matches;
	while (std::regex_search(source, matches, std::regex(control_regex))) 
	{
		result.emplace_back(matches[n]); // Capture the first (and only) submatch that holds the whole number only
		source = matches.suffix().str();
	}

	return result;
}

void Common::replaceAll(const std::string & from, const std::string & to, std::string & str) 
{
	if (from.empty())
	{
		return;
	}
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) 
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

template<>
void Common::increase<bool>(std::vector<bool>::reference val) 
{
    val = true;
}
