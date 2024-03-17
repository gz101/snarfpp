/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include <iostream>
#include <vector>
#include <cstdint> // For std::uint64_t
#include "../include/snarf.hpp"


int main() {
    //----------------------------------------
    // GENERATING DATA
    //----------------------------------------

    const std::uint64_t N = 10000000;
    std::vector<std::uint64_t> v_keys(N);
    for (std::uint64_t i = 0; i < N; ++i) {
        v_keys[i] = i * 10000;
    }

    //----------------------------------------
    // SNARF CONSTRUCTION
    //----------------------------------------

    const double bits_per_key = 10.0;
    const size_t batch_size = 100;
    const size_t R = 10000;

    SNARF<std::uint64_t> snarf_instance = SNARF<std::uint64_t>(
        v_keys, bits_per_key, batch_size, R
    );

    size_t snarf_size = snarf_instance.size_bytes();
    std::cout << "Bits per key used: " << snarf_size * 8.0 / v_keys.size()
        << std::endl;

    //----------------------------------------
    // QUERYING SNARF
    //----------------------------------------

    // Start with left and right out of range.
    std::uint64_t left = 15000;
    std::uint64_t right = 16000;

    // Should be true negative.
    if (snarf_instance.range_query(left, right)) {
        std::cout << "False Positive for [" << left << ", " << right << "]"
            << std::endl;
    } else {
        std::cout << "True Negative for [" << left << ", " << right << "]"
            << std::endl;
    }

    // Should be true positive.
    right = 25000;
    if (snarf_instance.range_query(left, right)) {
        std::cout << "True Positive for [" << left << ", " << right << "]"
            << std::endl;
    } else {
        std::cout << ":( False Negative for [" << left << ", " << right << "]"
            << std::endl;
    }

    // Should be false positive.
    left = 10001;
    right = 16000;
    if (snarf_instance.range_query(left, right)) {
        std::cout << "False Positive for [" << left << ", " << right << "]"
            << std::endl;
    } else {
        std::cout << "True Negative for [" << left << ", " << right << "]"
            << std::endl;
    }

    // No false negatives. Should be true positive.
    left = 10000;
    right = 20000;
    if (snarf_instance.range_query(left, right)) {
        std::cout << "True Positive for [" << left << ", " << right << "]"
            << std::endl;
    } else {
        std::cout << ":( False Negative for [" << left << ", " << right << "]"
            << std::endl;
    }

    return 0;
}
