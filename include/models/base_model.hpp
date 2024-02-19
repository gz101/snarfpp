/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
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

    // Array of <key, eCDF> pairs from the input data set chosen to construct
    // CDF model.
    KeyCDFPairList _key_array;

    // BaseModel(input_keys, R)
    //   Constructs the eCDF model given the entire set of input keys. Includes
    //   building the array of chosen keys and specified model. Assumes the
    //   input keys are in sorted order.
    BaseModel(const std::vector<Key>& input_keys, size_t R) {
        if (R > input_keys.size()) {
            throw std::runtime_error(
                "ERROR: `R` value larger than training data size."
            );
        }

        // A list of <key, eCDF> pairs of the input data set.
        KeyCDFPairList training_data;
        _compute_ecdf(input_keys, training_data);

        // Sample the training data using the input parameter R.
        _build_key_array(training_data, R);

        // constructing the model is handled by child class
    }

    // _compute_ecdf(input_keys, training_data)
    //   Computes the eCDF of every input key in the training data. Assumes the
    //   input keys are given in sorted order.
    void _compute_ecdf(
        const std::vector<Key>& input_keys, KeyCDFPairList& training_data
    ) {
        size_t input_keys_size = input_keys.size();
        training_data.resize(input_keys_size);

        // construct the eCDF list of keys
        for (size_t i = 0; i < input_keys_size; ++i) {
            // eCDF is equi-distant for each key
            training_data[i] = std::make_pair(
                input_keys[i], (i + 1) * 1.0 / input_keys_size
            );
        }
    }

    // _build_key_array(training_data, R)
    //   Constructs the key array based on the input key array and interval
    //   R to  select keys.
    void _build_key_array(const KeyCDFPairList& training_data, size_t R) {
        size_t training_data_size = training_data.size();
        size_t key_array_size = ceil(training_data_size * 1.0 / R);

        // we use N/R models, so every R-th key is sampled
        this->_key_array.resize(key_array_size);
        for (size_t i = 0; i < key_array_size; ++i) {
            KeyCDFPair pair = training_data[
                int(((i + 1) * training_data_size * 1.0) / key_array_size) - 1
            ];
            this->_key_array[i] = std::make_pair(
                std::move(pair.first), std::move(pair.second)
            );
        }

        // add the final key to the chosen key array
        this->_key_array[key_array_size - 1] = training_data[
            training_data_size - 1
        ];
    }

    // predict(key)
    //   Given an input key, returns the CDF of the key, based on the specified
    //   model.
    virtual double predict(Key key) = 0;

    // size()
    //   Returns the size of the model in bytes.
    virtual size_t size_bytes() = 0;

    // print_model()
    //   Prints the model to console in human-readable format. Used mainly for
    //   debugging purposes.
    virtual void print_model() = 0;
};
