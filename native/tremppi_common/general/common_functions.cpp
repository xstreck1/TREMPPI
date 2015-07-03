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
		return;
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
