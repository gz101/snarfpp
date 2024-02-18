/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "../include/base_test_utils.hpp"


void TestLinearSplineModel::test_calculate_slope_and_bias() {
    LinearSplineModel<int> model({1, 2, 3}, 1);

    // Test 1: Positive slope.
    auto result = model._calculate_slope_bias({1, 2}, {3, 4});
    // Expected slope = 1, bias = 1
    assert(result.first == 1 && result.second == 1);

    // Test 2: Negative slope.
    result = model._calculate_slope_bias({2, 3}, {4, 1});
    // Expected slope = -1, bias = 5.
    assert(result.first == -1 && result.second == 5);
}


void TestLinearSplineModel::test_constructor() {
    std::vector<int> keys = {1, 2, 3, 4, 5};
    size_t R = 2;

    LinearSplineModel<int> model(keys, R);
    size_t expected_key_array_size = ceil(keys.size() * 1.0 / 2) + 1;

    // Verify the size of _linear_models_array is correct.
    assert(model._linear_models_array.size() == expected_key_array_size);
}


void TestLinearSplineModel::test_predict() {
    std::vector<int> keys = {0, 10};
    size_t R = 2;

    LinearSplineModel<int> model(keys, R);

    // Test predictions at various points
    assert(model.predict(0) == 0.0); // At the start of the range
    assert(model.predict(5) == 0.5); // Midway through the range
    assert(model.predict(10) == 1.0); // At the end of the range

    // Test a key outside the range.
    assert(model.predict(-1) == 0.0);
    assert(model.predict(20) == 1.0);
}


int TestLinearSplineModel::run_linear_spline_model_tests() {
    test_calculate_slope_and_bias();
    test_constructor();
    test_predict();

    std::cout << "All LinearSplineModel unit tests passed successfully.\n";
    return 0;
}
