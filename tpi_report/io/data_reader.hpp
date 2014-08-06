#pragma once
#include <tpi_common/database_reader.hpp>
#include "../data/data_storage.hpp"

namespace DataReader {
	// Read header for the parametrizations
	/*void readParInfo(const SQLAdapter & database, Par & par) {
		auto column_data = database.getColumnsByExpression(PARAMETRIZATIONS_TABLE, regex{ "K_.*" });
		par = Par{ column_data.first, column_data.second };
	}*/

	//
	/*void readRegData(const SQLAdapter & database, const string & select, Regs & regs) {
		for (Reg & reg : regs) {
			reg.params = DatabaseReader::readParams(reg.params_ids, database, select);
			reg.unique = reg.params;
			sort(WHOLE(reg.unique));
			reg.unique.resize(distance(begin(reg.unique), std::unique(WHOLE(reg.unique))));
		}
	}*/

	//
	/*void readParData(const SQLAdapter & database, const string & select, Par & par) {
		par.params = DatabaseReader::readParams(vector<size_t>({}), database, select);
		sort(WHOLE(par.params));
	}*/
}