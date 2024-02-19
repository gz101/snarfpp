/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "../include/base_test_utils.hpp"


void TestBitArray::test_constructor() {
    size_t size = 64;
    BitArray ba(size);
     // Verify the size is correctly set.
    assert(ba._bit_array.size() == size);

    // Verify that all bits are initialized to 0.
    for (size_t i = 0; i < size; ++i) {
        assert(ba.read_bit(i) == 0);
    }
}


void TestBitArray::test_write_and_read_bits() {
    BitArray ba(64); // create a bit array of size 64 bits

    // Write the binary of 15 (1111) into the first 4 bits.
    ba.write_bits(0, 15, 4);
    // Verify the read value matches the written value.
    assert(ba.read_bits(0, 4) == 15);
}


void TestBitArray::test_read_bit() {
    BitArray ba(64);
    ba.write_bits(5, 1, 1); // set the 6th bit
    assert(ba.read_bit(5) == true); // verify the bit is correctly read
}


void TestBitArray::test_size_bytes() {
    BitArray ba_1(64); // 64 bits should use 8 bytes
    assert(ba_1.size_bytes() == 8); // verify the size in bytes is correct

    BitArray ba_2(65); // 65 bits should use 9 bytes
    assert(ba_2.size_bytes() == 9);

    BitArray ba_3(63); // 63 bits should use 8 bytes
    assert(ba_3.size_bytes() == 8);
}


int TestBitArray::run_bit_array_tests() {
    test_constructor();
    test_write_and_read_bits();
    test_read_bit();
    test_size_bytes();

    std::cout << "All BitArray unit tests passed successfully.\n";
    return 0;
}
