/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "../include/base_test_utils.hpp"


void TestBaseModel::test_constructor_success_valid_inputs() {
    std::vector<int> input_keys = {1, 2, 3, 4, 5};
    size_t R = 2;
    MockModel<int> model(input_keys, R);

    assert(!model._key_array.empty());
    assert(model._key_array.size() == ceil(input_keys.size() * 1.0 / R));
}


void TestBaseModel::test_constructor_failure_large_R() {
    std::vector<int> input_keys = {1, 2, 3, 4, 5};
    size_t R = 10;

    try {
        MockModel<int> model(input_keys, R);
        assert(false);  // if it reaches here, the test should fail
    } catch (const std::runtime_error& e) {
        assert(true);   // expected path: exception thrown due to invalid R
    } catch (...) {
        assert(false);  // unexpected exception type
    }
}


void TestBaseModel::test_compute_ecdf_correctness() {
    std::vector<int> input_keys = {1, 2, 3, 4, 5};
    MockModel<int> model(input_keys, 1);

    // Check if eCDF is correctly computed.
    for (size_t i = 0; i < input_keys.size(); ++i) {
        double expected_ecdf = (i + 1) * 1.0 / input_keys.size();
        assert(model._key_array[i].second == expected_ecdf);
    }
}


void TestBaseModel::test_compute_ecdf_with_duplicates() {
    std::vector<int> input_keys = {1, 1, 2, 3, 3, 3};
    MockModel<int> model(input_keys, 1);

    assert(model._key_array.size() == input_keys.size());
    for (size_t i = 1; i < model._key_array.size(); ++i) {
        assert(model._key_array[i].second > model._key_array[i - 1].second);
    }
}


void TestBaseModel::test_compute_ecdf_boundary_conditions() {
    std::vector<int> input_keys = {42}; // Single key
    MockModel<int> model(input_keys, 1);

    assert(model._key_array.size() == 1);
    assert(model._key_array[0].second == 1.0); // eCDF of a single key must be 1
}


void TestBaseModel::test_build_key_array_correct_sampling() {
    std::vector<int> input_keys = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t R = 2;
    MockModel<int> model(input_keys, R);

    size_t expected_size = ceil(input_keys.size() * 1.0 / R);
    assert(model._key_array.size() == expected_size);

    // Verify that keys are correctly sampled.
    for (size_t i = 0; i < model._key_array.size(); ++i) {
        assert(model._key_array[i].first == input_keys[R + i * R - 1]);
    }
}


void TestBaseModel::test_build_key_array_varying_R() {
    std::vector<int> input_keys = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Test with R = 1
    MockModel<int> model1(input_keys, 1);
    assert(model1._key_array.size() == input_keys.size());

    // Test with R = N
    size_t R = input_keys.size();
    MockModel<int> model2(input_keys, R);
    assert(model2._key_array.size() == 1);
}


void TestBaseModel::test_build_key_array_final_key_inclusion() {
    std::vector<int> input_keys = {1, 2, 3, 4, 5};
    size_t R = 2;
    MockModel<int> model(input_keys, R);

    assert(model._key_array.back().first == input_keys.back());
}


int TestBaseModel::run_base_model_tests() {
    test_constructor_success_valid_inputs();
    test_constructor_failure_large_R();
    test_compute_ecdf_correctness();
    test_compute_ecdf_with_duplicates();
    test_compute_ecdf_boundary_conditions();
    test_build_key_array_correct_sampling();
    test_build_key_array_varying_R();
    test_build_key_array_final_key_inclusion();

    std::cout << "All BaseModel unit tests passed successfully.\n";
    return 0;
}
