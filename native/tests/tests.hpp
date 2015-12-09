#pragma once

#include "common_tests_data.hpp"

int basic_express_test();
int basic_bias_test();
int basic_correlations_test();
int basic_init_test();
int basic_update_test();
int basic_qualitative_test();
int basic_quantitative_test();
int basic_regulations_test();
int basic_impact_test();
int basic_sign_test();
int basic_spawn_test();
int basic_validate_test();
int basic_witness_test();

void createModel(const bfs::path & example_model_path);
void createSelect(const bfs::path & example_model_path);
void createProperties(const bfs::path & example_model_path);