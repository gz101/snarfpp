/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "../include/base_test_utils.hpp"


int main() {
    assert(TestBaseModel().run_base_model_tests() == 0);
    assert(TestBaseSplineModel().run_base_spline_model_tests() == 0);
    assert(TestLinearSplineModel().run_linear_spline_model_tests() == 0);
    assert(TestBitArray().run_bit_array_tests() == 0);
    assert(TestSNARF().run_snarf_tests() == 0);

    std::cout << "All tests passed :)" << std::endl;
}
