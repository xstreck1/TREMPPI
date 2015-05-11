#pragma once

#include <gtest/gtest.h>

#include <tremppi_common/header.h>
#include <tremppi_common/network/data_info.hpp>
#include <tremppi_common/general/system.hpp>

const string TEST_FOLDER = "tests";
const string TEST_PROJECT = "test_project";


class CommonTest : public ::testing::Test {
protected:
	RegInfos r_negative_loop; // A single loop on A
	RegInfos r_unregulated; // A unregulated
	RegInfos r_two_circuit; // A-B circuit

	void SetUp() override {
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
	}
};

class ArgPtr {
	int argc;
	char ** argv;

public:
	ArgPtr(const vector<string> & values) {
		argv = new char*[values.size() + 1];

		// Add binary name
		argv[0] = new char[TremppiSystem::BIN_PATH.string().size() + 1];
		strcpy(argv[0], TremppiSystem::BIN_PATH.string().c_str());

		// Copy new arguments
		argc = 1;
		for (const string & val : values) {
			argv[argc] = new char[val.size() + 1];
			strcpy(argv[argc], val.c_str());
			argc++;
		}
	}

	~ArgPtr() {
		for (int i = 0; i < argc; i++) {
			delete[] argv[i];
		}
		delete[] argv;
	}

	char ** getArgv() { return argv; }

	int getArgc() { return argc; }
};
