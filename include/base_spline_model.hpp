/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#pragma once

#define SEARCH_LIMIT 10

#include "base_model.hpp"


// BaseSplineModel
//   A `BaseSplineModel` interface for a spline model that can be used to
//   generate and search the selected key array from training data.
template <typename Key>
struct BaseSplineModel : BaseModel<Key> {
    // Array of keys chosen to construct CDF model.
    std::vector<Key> key_array;

    // BaseSplineModel(input_keys, R)
    //   Constructs the key array based on the input key array and interval
    //   R to  select keys. Assumes the training data is given in sorted order
    //   by key size.
    BaseSplineModel(
        const std::vector<Key>& input_keys, size_t R
    ) : BaseModel<Key>(input_keys, R) {
        size_t num_models = ceil(this->size * 1.0 / R);

        // we use N/R models, so every R-th key is sampled
        key_array.resize(num_models);
        for (size_t i = 0; i < num_models; ++i) {
            key_array[i] = input_keys[
                int(((i + 1) * this->size * 1.0) / num_models) - 1
            ];
        }

        // add the final key to the chosen key array
        key_array[num_models - 1] = input_keys[this->size - 1];

        // array of models is handled by child class
    }

    // binary_search(key)
    //   Iterative binary search used to determine which spline model they input
    //   key is located at.
    size_t binary_search(Key key) {
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
    }
};
