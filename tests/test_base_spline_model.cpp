/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "../include/base_test_utils.hpp"


void TestBaseSplineModel::test_binary_search() {
    std::vector<int> input_keys = {1, 3, 5};
    MockBaseSplineModel<int> model(input_keys, 1); // R is irrelevant here

    assert(model.binary_search(0) == 0); // below range
    assert(model.binary_search(1) == 0); // first element
    assert(model.binary_search(2) == 1); // between first and second
    assert(model.binary_search(3) == 1); // second element
    assert(model.binary_search(5) == 2); // last element
    assert(model.binary_search(6) == 2); // above range
}


int TestBaseSplineModel::run_base_spline_model_tests() {
    test_binary_search();

    std::cout << "All BaseSplineModel unit tests passed successfully.\n";
    return 0;
}
