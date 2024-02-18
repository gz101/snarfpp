/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#pragma once

#include <vector>
#include <utility>
#include <cmath>
#include <stdexcept>


// BaseModel
//   A `BaseModel` interface for a learned model that can be used to
//   predict the CDF of a key, given a training data set to learn from.
template <typename Key>
struct BaseModel {
    // Data representation as <key, eCDF> pair.
    typedef std::pair<Key, double> KeyCDFPair;
    // Collection of <key, eCDF> pairs.
    typedef std::vector<KeyCDFPair> KeyCDFPairList;

    // A list of <key, eCDF> pairs of the input data set.
    KeyCDFPairList _training_data;
    // The number of input keys.
    size_t _size;

    // BaseModel(input_keys, R)
    //   Constructs the eCDF model given the entire set of input keys. Includes
    //   building the array of chosen keys and specified model. Assumes the
    //   input keys are in sorted order.
    BaseModel(const std::vector<Key>& input_keys, size_t R) {
        _size = input_keys.size();

        if (R > _size) {
            throw std::runtime_error(
                "ERROR: `R` value larger than training data size."
            );
        }

        _training_data.resize(_size);

        // construct the eCDF list of keys
        for (size_t i = 0; i < _size; ++i) {
            // eCDF is equi-distant for each key
            _training_data[i] = std::make_pair(
                input_keys[i], (i + 1) * 1.0 / _size
            );
        }

        // constructing the model is handled by child class
    }

    // predict(key)
    //   Given an input key, returns the CDF of the key, based on the specified
    //   model.
    virtual double predict(Key key) = 0;
};
