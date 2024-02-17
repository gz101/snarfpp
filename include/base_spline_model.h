/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#pragma once

#include "base_model.h"


// BaseSplineModel
//   A `BaseSplineModel` interface for a spline model that can be used to
//   generate and search the selected key array from training data.
template <typename Key>
struct BaseSplineModel : BaseModel<Key> {
    // Array of keys chosen to construct CDF model.
    std::vector<Key> key_array;

    // BaseSplineModel(training_data)
    //   Constructs the key array based on the input training data and interval
    //   R to  select keys. Assumes the training data is given in sorted order
    //   by key size.
    BaseSplineModel(
        typename BaseModel<Key>::TrainingData& training_data, size_t R
    ) {
        if (R > training_data.size()) {
            throw std::runtime_error(
                "ERROR: `R` value larger than training data size."
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

        // array of models is handled by child class
    }

    // binary_search(key)
    //   Iterative binary search used to determine which spline model they input
    //   key is located at.
    size_t binary_search(Key key);
};
