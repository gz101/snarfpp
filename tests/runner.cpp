/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "../include/base_test_utils.hpp"


int main() {
    TestBaseModel().run_base_model_tests();
    TestBaseSplineModel().run_base_spline_model_tests();
    TestLinearSplineModel().run_linear_spline_model_tests();

    std::cout << "All tests passed :)" << std::endl;
}
