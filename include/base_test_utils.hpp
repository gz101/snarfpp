/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#pragma once

#include <cassert>
#include <iostream>

#include "base_model.hpp"
#include "base_spline_model.hpp"


// TestBaseModel
//   Container that encapsulates all unit tests for the BaseModel.
struct TestBaseModel {
    // MockModel
    //   Mocking the `BaseModel` struct for testing.
    template <typename Key>
    struct MockModel : public BaseModel<Key> {
        // Inherit original constructor.
        using BaseModel<Key>::BaseModel;

        // Simple implementation only for mocking purposes.
        double predict(Key key) override {
            return key * 1.0;
        }
    };

    // test_empty_input()
    //   Tests the `BaseModel` with no input keys.
    void test_empty_input();

    // test_single_key()
    //   Tests the `BaseModel` correctly constructs an eCDF with a single input key.
    void test_single_key();

    // test_multiple_keys()
    //   Tests the `BaseModel` correctly constructs an eCDF with multiple keys.
    void test_multiple_keys();

    // run_base_model_tests()
    //   Helper function to run all tests in this struct.
    int run_base_model_tests();
};


// TestBaseSplineModel
//   Container that encapsulates all unit tests for the BaseSplineModel.
struct TestBaseSplineModel {
    // MockModel
    //   Mocking the `BaseSplineModel` struct for testing.
    template <typename Key>
    struct MockBaseSplineModel : public BaseSplineModel<Key> {
        // Inherit original constructor.
        using BaseSplineModel<Key>::BaseSplineModel;

        // Simple implementation only for mocking purposes.
        double predict(Key key) override {
            return key * 1.0;
        }
    };

    // test_constructor_valid_input()
    //   Test the constructor with valid input.
    void test_constructor_valid_input();

    // test_constructor_large_R()
    //   Test the constructor with R larger than training data size.
    void test_constructor_large_R();

    // test_binary_search()
    //   Test binary_search functionality.
    void test_binary_search();

    // run_base_model_tests()
    //   Helper function to run all tests in this struct.
    int run_base_spline_model_tests();
};
