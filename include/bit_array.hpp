/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#pragma once

#include <boost/dynamic_bitset.hpp>


// BitArray
//   A `BitArray` interface for the bit array used in SNARF. Relies on the boost
//   library's dynamic_bitset as a base.
struct BitArray {
    // The underlying bit array storage.
    boost::dynamic_bitset<> _bit_array;

    // BitArray()
    //   Default constructor with zero arguments.
    BitArray() {}

    // BitArray(size)
    //   Constructs the underlying bit array with a size of `size`. It
    //   initializes all bits to 0 to begin with.
    BitArray(size_t size) {
        _initialize_bit_array(size);
    }

    // _initialize_bit_array(size)
    //   Helper method that resizes the bit array for the default constructor.
    void _initialize_bit_array(size_t size) {
        this->_bit_array.resize(size, 0);
    }

    // write_bits(offset, value, num_bits)
    //   Writes a certain number of bits from a value at an offset that is
    //   specified by the input parameters.
    void write_bits(size_t offset, size_t value, size_t num_bits) {
        for (size_t i = 0; i < num_bits; ++i) {
            // Set the value in the bit array to value[i].
            if (value >> i & 1) {
                this->_bit_array.set(offset + i);
            }
        }
    }

    // read_bits(offset, num_bits)
    //   Returns a certain number of bits at an offset that is specified by the
    //   input parameters.
    size_t read_bits(size_t offset, size_t num_bits) {
        size_t read = 0;
        for (size_t i = 0; i < num_bits; ++i) {
            // Set read[i] to the (offset + i)-th position in the bit array.
            if (this->_bit_array.test(offset + i)) {
                read |= (1ULL << i);
            }
        }

        return read;
    }

    // read_bit(offset)
    //   Reads a single bit at a specified offset.
    bool read_bit(size_t offset) {
        return this->_bit_array.test(offset);
    }

    // size_bytes()
    //   Returns the space used by the structure in bytes.
    size_t size_bytes() const {
        // Rounds up the number of bytes.
        return (this->_bit_array.size() + 7) / 8;
    }
};
