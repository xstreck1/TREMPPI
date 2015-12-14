#pragma once

#include <gtest/gtest.h>

#include <tremppi_common/header.h>
#include <tremppi_common/network/data_info.hpp>
#include <tremppi_common/general/system.hpp>

const string TEST_FOLDER = "projects";
const string TEST_PROJECT = "test_project";

class CommonTest : public ::testing::Test 
{
protected:
	RegInfos r_negative_loop; // A single loop on A
	RegInfos r_unregulated; // A unregulated
	RegInfos r_two_circuit; // A-B circuit
	RegInfos r_multi_val; //E = {(u, 1, v), (v, 2, v), (v, 1, u)}

	void SetUp() override 
	{
		r_negative_loop.emplace_back(RegInfo{0,	"A", 1});
		r_negative_loop[0].columns = map<size_t, string>{ { 0, string("K_neg_loop_A_0") }, { 1, string("K_neg_loop_A_1") } };
		r_negative_loop[0].regulators = map<size_t, Levels>{ { 0, { 1 } } };
		r_negative_loop[0].requirements = map<size_t, vector<Levels> >{ { 0, { { 0 } } }, { 1, { { 1 } } } };

		r_unregulated.emplace_back(RegInfo{ 0, "A", 1 });
		r_unregulated[0].columns = map<size_t, string>{ { 0, string("K_A_") } };
		r_unregulated[0].regulators = map<size_t, Levels>();
		r_unregulated[0].requirements = map<size_t, vector<Levels> >{ { 0, { } }};

		r_two_circuit.emplace_back(RegInfo{ 0, "A", 1 });
		r_two_circuit.emplace_back(RegInfo{ 1, "B", 1 });
		r_two_circuit[0].columns = map<size_t, string>{ { 0, string("K_two_circuit_A_0") }, { 1, string("K_two_circuit_A_1") } };
		r_two_circuit[0].regulators = map<size_t, Levels>{ { 1, { 1 } } };
		r_two_circuit[0].requirements = map<size_t, vector<Levels> >{ { 0, { { 0, 1 }, { 0 } } }, { 1, { { 0, 1 }, { 1 } } } };
		r_two_circuit[1].columns = map<size_t, string>{ { 2, string("K_two_circuit_B_0") }, { 3, string("K_two_circuit_B_1") } };
		r_two_circuit[1].regulators = map<size_t, Levels>{ { 0, { 1 } } };
		r_two_circuit[1].requirements = map<size_t, vector<Levels> >{ { 2, { { 0 }, { 0, 1 } } }, { 3, { { 1 }, { 0, 1 } } } };

		r_multi_val.emplace_back(RegInfo{ 0, "u", 1 });
		r_multi_val.emplace_back(RegInfo{ 1, "v", 2 });
		r_multi_val[0].columns = map<size_t, string>{ { 0, string("K_multi_val_u_0") },{ 1, string("K_multi_val_u_0") } };
		r_multi_val[0].regulators = map<size_t, Levels>{ { 1,{ 1 } } };
		r_multi_val[0].requirements = map<size_t, vector<Levels> >{ { 0,{ { 0, 1 },{ 0 } } },{ 1,{ { 0, 1 },{ 1, 2 } } } };
		r_multi_val[1].columns = map<size_t, string>{ { 2, string("K_multi_val_v_00") },{ 3, string("K_multi_val_v_02") },{4, string("K_multi_val_v_10") },{ 5, string("K_multi_val_v_12") } };
		r_multi_val[1].regulators = map<size_t, Levels>{ { 0,{ 1 } },{ 1,{ 2} } };
		r_multi_val[1].requirements = map<size_t, vector<Levels> >{ { 2,{ { 0 },{ 0, 1 } } }, { 3,{ { 0 },{ 2 } } }, { 4,{ { 1 },{ 0, 1 } } }, { 5,{ { 1 },{ 2 } } } };
	}
};
