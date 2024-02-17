/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#define SEARCH_LIMIT 10

#include "../include/base_spline_model.h"


template <typename Key>
BaseSplineModel<Key>::BaseSplineModel(
    typename BaseModel<Key>::TrainingData& training_data, size_t R
) {
    if (beta > training_data.size()) {
        throw std::runtime_error(
            "ERROR: `beta` value larger than training data size."
        );
    }

    // we use N/R models, so every R-th key is sampled
    size_t curr = 0;
    while (curr < key_array.size()) {
        curr += R;
        key_array.push_back(training_data[curr].first);
    }

    // add the final key to the chosen key array
    Key last = training_data.back().first;
    if (key_array.back() != last) {
        key_array.push_back(last);
    }
};


template <typename Key>
size_t BaseSplineModel<Key>::binary_search(Key key) {
    size_t left = 0;
    size_t right = key_array.size() - 1;
    size_t mid = left + ((right - left) >> 1);

    // binary search until <= 10 elements remain
    while ((right - left) > SEARCH_LIMIT) {
        if (key_array[mid] < key) {
            left = mid;
        } else {
            right = mid;
        }
        mid = left + ((right - left) >> 1);
    }

    // linear search final <= 10 elements
    for (size_t i = left; i <= right; ++i) {
        // return first key greater than or equal to the input key
        if (key_array[i] >= key) {
            return i;
        }
    }

    // the input key is greater than the last key in the array
    return key_array.size() - 1;
};
