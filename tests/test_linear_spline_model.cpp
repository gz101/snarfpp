/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "../include/base_test_utils.hpp"


void TestLinearSplineModel::test_calculate_slope_and_bias() {
    LinearSplineModel<int> model({1, 2, 3}, 1);

    // Test 1: Positive slope.
    auto result = model._calculate_slope_bias({1, 2}, {3, 4});
    // Expected slope = 1, bias = 1
    assert_double_equals(result.first, 1.0);
    assert_double_equals(result.second, 1.0);

    // Test 2: Negative slope.
    result = model._calculate_slope_bias({2, 3}, {4, 1});
    // Expected slope = -1, bias = 5.
    assert_double_equals(result.first, -1.0);
    assert_double_equals(result.second, 5.0);
}


void TestLinearSplineModel::test_constructor() {
    std::vector<int> keys = {1, 2, 3, 4, 8};
    size_t beta = 2;

    LinearSplineModel<int> model(keys, beta);
    size_t expected_key_array_size = ceil(keys.size() * 1.0 / 2) + 1;

    // Verify the size of _linear_models_array is correct.
    assert(model._linear_models_array.size() == expected_key_array_size);

    // Check spline points correctly constructed
    assert_double_equals(model._linear_models_array[0].first, 0.2);     // slope
    assert_double_equals(model._linear_models_array[0].second, 0.0);    // bias
    assert_double_equals(model._linear_models_array[1].first, 0.2);     // slope
    assert_double_equals(model._linear_models_array[1].second, 0.0);    // bias
    assert_double_equals(model._linear_models_array[2].first, 0.08);    // slope
    assert_double_equals(model._linear_models_array[2].second, 0.36);   // bias
}


void TestLinearSplineModel::test_predict() {
    std::vector<int> keys = {0, 10};
    size_t beta = 2;   // 1 model only from [0, 10]

    LinearSplineModel<int> model(keys, beta);

    // Test predictions at various points
    assert_double_equals(model.predict(0), 0.0); // At the start of the range
    assert_double_equals(model.predict(5), 0.5); // Midway through the range
    assert_double_equals(model.predict(10), 1.0); // At the end of the range

    // Test a key outside the range.
    assert_double_equals(model.predict(-1), 0.0);
    assert_double_equals(model.predict(20), 1.0);
}


void TestLinearSplineModel::test_paper_model() {
    std::vector<int> keys = {3, 5, 12, 13, 25, 35, 47, 57, 67, 72, 75, 80};
    size_t beta = 3;

    LinearSplineModel<int> model(keys, beta);

    // Test predictions at spline points
    assert_double_equals(model.predict(12), 0.25);
    assert_double_equals(model.predict(35), 0.5);
    assert_double_equals(model.predict(67), 0.75);
    assert_double_equals(model.predict(80), 1.0);

    // Test predictions between spline points
    assert_double_equals(model.predict(6), 0.125);
    assert_double_equals(model.predict(30), 0.4456);
    assert_double_equals(model.predict(53), 0.6406);
    assert_double_equals(model.predict(77), 0.9423);
}


int TestLinearSplineModel::run_linear_spline_model_tests() {
    test_calculate_slope_and_bias();
    test_constructor();
    test_predict();
    test_paper_model();

    std::cout << "All LinearSplineModel unit tests passed successfully.\n";
    return 0;
}
