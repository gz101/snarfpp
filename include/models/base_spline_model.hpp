/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
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
    typename BaseModel<Key>::KeyCDFPairList _key_array;

    // BaseSplineModel(input_keys, R)
    //   Constructs the key array based on the input key array and interval
    //   R to  select keys. Assumes the training data is given in sorted order
    //   by key size.
    BaseSplineModel(
        const std::vector<Key>& input_keys, size_t R
    ) : BaseModel<Key>(input_keys, R) {
        size_t key_array_size = ceil(this->_input_key_set_size * 1.0 / R);

        // we use N/R models, so every R-th key is sampled
        this->_key_array.resize(key_array_size);
        for (size_t i = 0; i < key_array_size; ++i) {
            typename BaseModel<Key>::KeyCDFPair pair = this->_training_data[
                int(
                    ((i + 1) * this->_input_key_set_size * 1.0) / key_array_size
                ) - 1
            ];
            this->_key_array[i] = std::make_pair(
                std::move(pair.first), std::move(pair.second)
            );
        }

        // add the final key to the chosen key array
        this->_key_array[key_array_size - 1] = this->_training_data[
            this->_input_key_set_size - 1
        ];

        // array of models is handled by child class
    }

    // binary_search(key)
    //   Iterative binary search used to determine which spline model they input
    //   key is located at.
    size_t binary_search(Key key) {
        size_t left = 0;
        size_t right = this->_key_array.size() - 1;

        // binary search until <= 10 elements remain
        while ((right - left) > SEARCH_LIMIT) {
            size_t mid = left + ((right - left) >> 1);

            if (this->_key_array[mid].first < key) {
                left = mid;
            } else {
                right = mid;
            }
        }

        // linear search final <= 10 elements
        for (size_t i = left; i <= right; ++i) {
            // return first key greater than or equal to the input key
            if (this->_key_array[i].first >= key) {
                return i;
            }
        }

        // the input key is greater than the last key in the array
        return this->_key_array.size() - 1;
    }
};
