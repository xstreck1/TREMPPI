#pragma once
#include <tremppi_common/database/sqlite3pp_func.hpp>
#include <tremppi_common/general/common_functions.hpp>
#include <tremppi_common/report/report.hpp>
#include <tremppi_common/general/logging.hpp>

#include "statistical_analysis.hpp"
#include "../data/data_storage.hpp"

namespace Compute
{
	FunData build(const RegInfo & info, const int step_count, sqlite3pp::query & qry);

	void deviation(const RegInfos & reg_infos, const size_t step_count, vector<sqlite3pp::query> & queries, Logging & logging, FunsData & funs_data);

	void correlation(const RegInfos & reg_infos, const size_t step_count, vector<sqlite3pp::query> & queries, Logging & logging, FunsData & funs_data);
}