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

#include "file_manipulation.hpp"

#include "../general/time_manager.hpp"
#include "../general/system.hpp"

void FileManipulation::copyDir(bfs::path const & source, bfs::path const & destination)
{
	// Check whether the function call is valid
	if (!bfs::exists(source) || !bfs::is_directory(source))	
	{
		throw runtime_error("Source directory " + source.string() + " does not exist or is not a directory.");
	}
	//
	if (bfs::exists(destination)) 
	{
		throw runtime_error("Destination directory " + destination.string() + " already exists.");
	}
	// Create the destination directory
	if (!bfs::create_directory(destination)) 
	{
		throw runtime_error("Unable to create destination directory" + destination.string());
	}
	// Iterate through the source directory
	for (bfs::directory_iterator file(source); file != bfs::directory_iterator(); ++file) 
	{
		bfs::path current(file->path());
		if (bfs::is_directory(current)) 
		{
			// Found directory: Recursion
			copyDir(current, destination / current.filename());
		}
		else 
		{
			// Found file: Copy
			bfs::copy_file(current, destination / current.filename());
		}
	}
}
Json::Value FileManipulation::parseJSON(const bfs::path & input_path) 
{
	Json::Value root;

	// Open file
	ifstream file(input_path.string(), ios::in);
	if (!file)
	{
		throw runtime_error("Could not open the file " + input_path.string() + " for reading.");
	}

	// Parse JSON
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(file, root);
	if (!parsingSuccessful)
	{
		throw runtime_error("Failed to parse configuration. " + reader.getFormattedErrorMessages());
	}

	return root;
}

void FileManipulation::writeJSON(const bfs::path & output_path, const Json::Value & data) 
{
	fstream data_file(output_path.string(), ios::out);
	if (!data_file)
	{
		throw runtime_error("Could not open " + output_path.string());
	}

	Json::StyledWriter writer;
	string result = writer.write(data);
	data_file << result << endl;
}

void FileManipulation::replaceInFile(bfs::path const & file, const string & original, const string & replacement) 
{
	bfs::path out_file = file;
	out_file.replace_extension(TimeManager::getTimeStamp());
	fstream fin(file.string(), ios::in);
	fstream fout(out_file.string(), ios::out);

	string line;
	while (getline(fin, line)) 
	{
		replaceAll(original, replacement, line);
		fout << line << endl;
	}

	fout.close(); fin.close();
	bfs::remove(file);
	bfs::copy(out_file, file);
	bfs::remove(out_file);
}
