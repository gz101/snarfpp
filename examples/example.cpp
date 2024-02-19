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

    const std::uint64_t N = 10'000'000;
    std::vector<std::uint64_t> v_keys(N);
    for (std::uint64_t i = 0; i < N; ++i) {
        v_keys[i] = i * 10000;
    }

    //----------------------------------------
    // SNARF CONSTRUCTION
    //----------------------------------------

    const double bits_per_key = 10.00;
    const int batch_size = 100;
    const size_t R = 1000;

    SNARF<std::uint64_t> snarf_instance = SNARF<std::uint64_t>(
        v_keys, bits_per_key, batch_size, R
    );

    size_t snarf_size = snarf_instance.size();
    std::cout << "Bits per key used: " << snarf_size * 8.0 / v_keys.size()
        << std::endl;

    //----------------------------------------
    // QUERYING SNARF
    //----------------------------------------

    auto perform_query = [&](
        std::uint64_t left_end,
        std::uint64_t right_end,
        const std::string& expectation
    ) {
        if (snarf_instance.range_query(left_end, right_end)) {
            std::cout << "False Positive for: [ " << left_end << ", "
                << right_end << "]" << std::endl;
        } else {
            std::cout << expectation << " for: [ " << left_end << ", "
                << right_end << "]" << std::endl;
        }
    };

    std::uint64_t left_end = 15000, right_end = 16000;
    perform_query(left_end, right_end, "True Positive");

    // snarf_instance.insert_key(15000);
    // perform_query(left_end, right_end, "True Positive");

    // snarf_instance.delete_key(15000);
    // perform_query(left_end, right_end, "True Negative");

    std::uint64_t false_positive_left_end = 10001;
    perform_query(false_positive_left_end, right_end, "True Negative");

    return 0;
}
