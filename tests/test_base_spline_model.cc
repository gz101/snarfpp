/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "../include/base_test_utils.h"


void TestBaseSplineModel::test_constructor_valid_input() {
    std::vector<int> input_keys = {1, 2, 3, 4, 5};
    size_t R = 2;
    MockBaseSplineModel<int> model(input_keys, R);

    assert(model.key_array.size() == 3);
    assert(model.key_array[0] == 1);
    assert(model.key_array[1] == 3);
    assert(model.key_array[2] == 5);
}


void TestBaseSplineModel::test_constructor_large_R() {
    std::vector<int> input_keys = {1, 2};
    try {
        size_t R = 3; // larger than training data size
        MockBaseSplineModel<int> model(input_keys, R);
        assert(false); // should not reach this line
    } catch (const std::runtime_error& e) {
        // expected path
    } catch (...) {
        assert(false); // unexpected exception type
    }
}


void TestBaseSplineModel::test_binary_search() {
    std::vector<int> input_keys = {1, 2, 3, 4, 5};
    MockBaseSplineModel<int> model(input_keys, 1); // R is irrelevant here

    assert(model.binary_search(0) == 0); // below range
    assert(model.binary_search(1) == 0); // first element
    assert(model.binary_search(2) == 1); // between first and second
    assert(model.binary_search(3) == 1); // second element
    assert(model.binary_search(5) == 2); // last element
    assert(model.binary_search(6) == 2); // above range
}


int TestBaseSplineModel::run_base_spline_model_tests() {
    test_constructor_valid_input();
    test_constructor_large_R();
    test_binary_search();

    std::cout << "All BaseSplineModel unit tests passed successfully.\n";
    return 0;
}
