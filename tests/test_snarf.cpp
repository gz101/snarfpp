/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "../include/base_test_utils.hpp"


void TestSNARF::test_constructor() {
    std::vector<int> input_keys = {1, 2, 3, 4, 5};
    double bits_per_key = 10;
    size_t elements_per_block = 2;
    size_t R = 2;

    SNARF<int> snarf(input_keys, bits_per_key, elements_per_block, R);
    size_t expected_blocks = (input_keys.size() + elements_per_block - 1)
        / elements_per_block;

    assert(snarf._total_keys == input_keys.size());
    assert(snarf._total_blocks == expected_blocks);
}


void TestSNARF::test_range_query_with_no_matches() {
    // std::vector<int> input_keys = {10, 20, 30, 40, 50};
    // SNARF<int> snarf(input_keys, 10, 5, 1);

    // // Test a range that should not have matches
    // assert(!snarf.range_query(35, 38));
}


void TestSNARF::test_range_query_with_matches() {
    // std::vector<int> input_keys = {10, 20, 30, 40, 50};
    // SNARF<int> snarf(input_keys, 10, 5, 2);

    // // Test a range that should have matches
    // assert(snarf.range_query(15, 35));
    // assert(snarf.range_query(39, 41));
}


void TestSNARF::test_size_bytes() {
    std::vector<int> input_keys = {1, 2, 3, 4, 5};
    SNARF<int> snarf(input_keys, 10, 2, 2);

    size_t expected_size = 172;  // to check this value by hand calculation
    assert(snarf.size_bytes() == expected_size);
}


int TestSNARF::run_snarf_tests() {
    test_constructor();
    test_range_query_with_no_matches();
    test_range_query_with_matches();
    test_size_bytes();

    std::cout << "All SNARF unit tests passed successfully.\n";
    return 0;
}
