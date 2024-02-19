/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
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
#include "bit_array.hpp"
#include "snarf.hpp"


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
            return key * 0.1 / key;
        }

        // Simple implementation only for mocking purposes.
        size_t size_bytes() override {
            return 16;
        }

        // Simple implementation only for mocking purposes.
        void print_model() override {}
    };

    // test_constructor_success_valid_inputs()
    //   Tests the model initializes correctly with valid inputs.
    void test_constructor_success_valid_inputs();

    // test_constructor_failure_large_R()
    //   Tests that the model correctly throws an error for excessive R.
    void test_constructor_failure_large_R();

    // test_compute_ecdf_correctness()
    //   Tests that the eCDF is correctly computed for valid inputs.
    void test_compute_ecdf_correctness();

    // test_compute_ecdf_with_duplicates()
    //   Checks the increasing nature of eCDF values given duplicate keys.
    void test_compute_ecdf_with_duplicates();

    // test_compute_ecdf_boundary_conditions()
    //   Tests the cases with a single key as input.
    void test_compute_ecdf_boundary_conditions();

    // test_build_key_array_correct_sampling()
    //   Verify keys are correctly sampled according to the input R value.
    void test_build_key_array_correct_sampling();

    // test_build_key_array_varying_R()
    //   Checks behavior when R = 1 and R = N boundary conditions.
    void test_build_key_array_varying_R();

    // test_build_key_array_final_key_inclusion()
    //   Checks that the final key is included in the sampled set.
    void test_build_key_array_final_key_inclusion();

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
            return key * 0.1 / key;
        }

        // Simple implementation only for mocking purposes.
        size_t size_bytes() override {
            return 16;
        }

        // Simple implementation only for mocking purposes.
        void print_model() override {}
    };

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

    // test_size_bytes()
    //   Tests that the size_bytes() function correctly calculates the model
    //   size.
    void test_size_bytes();

    // test_paper_model()
    //   Recreates example from the original SNARF paper for testing.
    void test_paper_model();

    // run_linear_spline_model_tests()
    //   Helper function to run all tests in this struct.
    int run_linear_spline_model_tests();
};


// TestBitArray
//   Container that encapsulates all unit tests for the BitArray struct.
struct TestBitArray {
    // test_default_constructor()
    //   Tests the zero-argument default constructor.
    void test_default_constructor();

    // test_initialize_bit_array()
    //   Tests correct initialization of the bit array with an input size.
    void test_initialize_bit_array();

    // test_constructor()
    //   Tests the constructor correctly initializes to the specified size.
    void test_constructor();

    // test_write_and_read_bits()
    //   Tests that the write and read operations work correctly together.
    void test_write_and_read_bits();

    // test_read_bits()
    //   Tests that the read function correctly returns the right bits.
    void test_read_bit();

    // test_size_bytes()
    //   Tests that the correct size in bytes are returned.
    void test_size_bytes();

    // run_bit_array_tests()
    //   Helper function to run all tests in this struct.
    int run_bit_array_tests();
};


// TestSNARF
//   Container that encapsulates all unit tests for the SNARF struct.
struct TestSNARF {
    // test_constructor()
    //   Tests the SNARF constructor to ensure correct initialization of
    //   internal state.
    void test_constructor();

    // test_set_golomb_parameters()
    //   Verifies that the Golomb parameter is correctly set.
    void test_set_golomb_parameters();
    
    // test_set_locations()
    //   Checks if the locations array is correctly populated.
    void test_set_locations();

    // test_build_bit_blocks()
    //   Verifies that blocks are correctly constructed and partitioned.
    void test_build_bit_blocks();

    // test_create_gcs_block()
    //   Checks the encoding of a simple batch into a Golomb-coded sequence.
    void test_create_gcs_block();

    // test_range_query()
    //   Checks it correctly identifies the presence (or absence) of keys within
    //   a specified range.
    void test_range_query();

    // test_query_block()
    //   Verifies it can identify the presence of a key within a block.
    void test_query_block();

    // run_snarf_tests()
    //   Helper function to run all tests in this struct.
    int run_snarf_tests();
};


inline void assert_double_equals(double x, double y) {
    assert(fabs(x - y) < EPS);
}
