#include "common_functions.hpp"

template<>
void Common::increase<bool>(std::vector<bool>::reference val) { val = true; }

std::vector<std::string> Common::getAllMatches(const std::string & control_regex, const std::string & original, const int n) {
	std::vector<std::string> result;

	std::string source = original;
	std::smatch matches;
	while (std::regex_search(source, matches, std::regex(control_regex))) {
		result.emplace_back(matches[n]); // Capture the first (and only) submatch that holds the whole number only
		source = matches.suffix().str();
	}

	return result;
}