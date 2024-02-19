/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "../include/base_test_utils.hpp"


void TestSNARF::test_constructor() {
    std::vector<int> input_keys = {1, 2, 3, 4, 5};
    double bits_per_key = 10.0;     // Example configuration.
    size_t elements_per_block = 2;  // Example block size configuration.
    
    SNARF<int> snarf(input_keys, bits_per_key, elements_per_block);
    
    // Verify the total number of keys is correctly initialized.
    assert(snarf._total_keys == input_keys.size());
    // Verify the block size is set as per the constructor argument.
    assert(snarf._block_size == elements_per_block);
    // The total blocks should be the ceiling of total keys divided by elements
    // per block.
    size_t expected_total_blocks = ceil(
        input_keys.size() * 1.0 / elements_per_block
    );
    assert(snarf._total_blocks == expected_total_blocks);

    // Verify Golomb parameter initialization based on bits_per_key logic
    // provided in _set_golomb_parameters.
    double target_FPR = pow(0.5, bits_per_key - 3.0);
    size_t expected_golomb_param = pow(2, ceil(log2(1.0 / target_FPR)));
    assert(snarf._golomb_param == expected_golomb_param);

    // Verify bit size is correctly calculated and set.
    assert(snarf._bit_size == ceil(log2(1.0 / target_FPR)));
    // Validate that the bit set array is resized to accommodate the total 
    // number of blocks.
    assert(snarf._bit_set_array.size() == expected_total_blocks);
}


void TestSNARF::test_set_golomb_parameters() {
    SNARF<int> snarf({}, 10, 100);
    snarf._set_golomb_parameters(10);

    // Expected Golomb parameter based on the bits_per_key.
    assert(snarf._golomb_param == 1024);
    // The bit size should match the input parameter for this configuration.
    assert(snarf._bit_size == 10);
}


void TestSNARF::test_set_locations() {

}


void TestSNARF::test_build_bit_blocks() {

}


void TestSNARF::test_create_gcs_block() {

}


void TestSNARF::test_range_query() {

}


void TestSNARF::test_query_block() {

}


int TestSNARF::run_snarf_tests() {
    test_constructor();
    test_set_golomb_parameters();
    test_set_locations();
    test_build_bit_blocks();
    test_create_gcs_block();
    test_range_query();
    test_query_block();

    std::cout << "All SNARF unit tests passed successfully.\n";
    return 0;
}
