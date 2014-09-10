#include "file_manipulation.hpp"

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

// Read the (almost) JSON file
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