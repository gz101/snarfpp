/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#pragma once

#define EPS 0.001

#include <cassert>
#include <iostream>
#include <cmath>

#include "models/base_model.hpp"
#include "models/base_spline_model.hpp"
#include "models/linear_spline_model.hpp"


// assert_double_equals(x, y)
//   Asserts that x and y are within EPS of each other to handle floating point
//   comparisons.
void assert_double_equals(double x, double y);


// TestBaseModel
//   Container that encapsulates all unit tests for the BaseModel.
struct TestBaseModel {
    // MockModel
    //   Mocking the `BaseModel` struct for testing.
    template <typename Key>
    struct MockModel : BaseModel<Key> {
        // Inherit original constructor.
        using BaseModel<Key>::BaseModel;

        // Simple implementation only for mocking purposes.
        double predict(Key key) override {
            return key * 1.0;
        }

        // Simple implementation only for mocking purposes.
        void print_model() {}
    };

    // test_empty_input()
    //   Tests the `BaseModel` with no input keys.
    void test_empty_input();

    // test_single_key()
    //   Tests the `BaseModel` correctly constructs an eCDF with an input key.
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
    struct MockBaseSplineModel : BaseSplineModel<Key> {
        // Inherit original constructor.
        using BaseSplineModel<Key>::BaseSplineModel;

        // Simple implementation only for mocking purposes.
        double predict(Key key) override {
            return key * 1.0;
        }

        // Simple implementation only for mocking purposes.
        void print_model() {}
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

    // run_base_spline_model_tests()
    //   Helper function to run all tests in this struct.
    int run_base_spline_model_tests();
};


// TestLinearSplineModel
//   Container that encapsulates all unit tests for the LinearSplineModel.
struct TestLinearSplineModel {
    // test_calculate_slope_and_bias()
    //   Tests that the slope and bias are correctly calculated.
    void test_calculate_slope_and_bias();

    // test_constructor()
    //   Tests that the linear spline array is correctly initialized.
    void test_constructor();

    // test_predict()
    //   Tests that the predict() function correctly interpolates the CDF model.
    void test_predict();

    // test_paper_model()
    //   Recreates example from the original SNARF paper for testing.
    void test_paper_model();

    // run_linear_spline_model_tests()
    //   Helper function to run all tests in this struct.
    int run_linear_spline_model_tests();
};


inline void assert_double_equals(double x, double y) {
    assert(fabs(x - y) < EPS);
}
