/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "../include/base_test_utils.h"


void TestBaseModel::test_empty_input() {
    MockModel<int> model(std::vector<int>{}, 0);
    assert(model.training_data.empty());
}


void TestBaseModel::test_single_key() {
    MockModel<int> model({1}, 0);
    assert(model.training_data.size() == 1);
    assert(model.training_data[0] == std::make_pair(1, 0.0));
}


void TestBaseModel::test_multiple_keys() {
    MockModel<int> model({1, 2, 3}, 0);
    assert(model.training_data.size() == 3);
    assert(model.training_data[0] == std::make_pair(1, 0.0));
    assert(model.training_data[1] == std::make_pair(2, 1.0 / 3));
    assert(model.training_data[2] == std::make_pair(3, 2.0 / 3));
}


int TestBaseModel::run_base_model_tests() {
    test_empty_input();
    test_single_key();
    test_multiple_keys();

    std::cout << "All BaseModel unit tests passed successfully.\n";
    return 0;
}
