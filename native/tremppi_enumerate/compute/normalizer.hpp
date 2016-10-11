#pragma once

#include <tremppi_common/database/database_reader.hpp>

class Normalizer {
private:
	vector<CompID> components_to_check;
	map<pair<size_t, bool>, int> neighbour_contexts; 

	// For the original regulatory context find a negibour context with the self regulation either higher (direction == true) or lower (direction == false)
	int find_neighbour(const Configurations & original, const bool direction, const RegInfo & reg_info);

	ActLevel normalize(const Levels & original, const RegInfo & reg_info, const size_t column, Levels & normalized);

public:
	Normalizer(const RegInfos & reg_infos);

	bool isNormalized(const Levels & parametrization, const RegInfos & reg_infos);
};