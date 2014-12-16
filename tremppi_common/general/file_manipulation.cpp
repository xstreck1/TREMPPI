#include "file_manipulation.hpp"

#include "../general/time_manager.hpp"

void FileManipulation::copyDir(bfs::path const & source, bfs::path const & destination)
{
	// Check whether the function call is valid
	if (!bfs::exists(source) || !bfs::is_directory(source))	{
		throw runtime_error("Source directory " + source.string() + " does not exist or is not a directory.");
	}
	if (bfs::exists(destination)) {
		throw runtime_error("Destination directory " + destination.string() + " already exists.");
	}
	// Create the destination directory
	if (!bfs::create_directory(destination)) {
		throw runtime_error("Unable to create destination directory" + destination.string());
	}
	// Iterate through the source directory
	for (bfs::directory_iterator file(source); file != bfs::directory_iterator(); ++file) {
		bfs::path current(file->path());
		if (bfs::is_directory(current)) {
			// Found directory: Recursion
			copyDir(current, destination / current.filename());
		}
		else {
			// Found file: Copy
			bfs::copy_file(current, destination / current.filename());
		}
	}
}

Json::Value FileManipulation::readJSasJSON(const bfs::path & input_path) {
	Json::Value root;

	// Open file
	ifstream file(input_path.string(), ios::in);
	if (!file)
		throw runtime_error("Could not open the file " + input_path.string() + " for reading.");

	// Remove JS additions
	std::stringstream buffer;
	char ch = static_cast<char>(0);
	while (ch != '{' && ch != '[' && ch != EOF) {
		file.get(ch);
	}
	do {
		buffer.put(ch);
		file.get(ch);
	} while (ch != ';' && file.good());

	file.close();

	// Parse JSON
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(buffer, root);
	if (!parsingSuccessful)
		throw runtime_error("Failed to parse configuration. " + reader.getFormattedErrorMessages());

	return root;
}


void FileManipulation::replaceInFile(bfs::path const & file, const string & original, const string & replacement) {
	bfs::path out_file = file;
	out_file.replace_extension(TimeManager::getTimeStamp());
	fstream fin(file.string(), ios::in);
	fstream fout(out_file.string(), ios::out);

	string line;
	while (getline(fin, line)) {
		replaceAll(original, replacement, line);
		fout << line << endl;
	}

	fout.close(); fin.close();
	bfs::remove(file);
	bfs::copy(out_file, file);
	bfs::remove(out_file);
}


//
ofstream FileManipulation::fileOutput(const bfs::path & file_path, const string & name) {
	bfs::path output_path = file_path / name;
	ofstream file = ofstream(output_path.string(), ios::out);
	if (!file)
		throw runtime_error("Could not open " + output_path.string());
	return file;
}