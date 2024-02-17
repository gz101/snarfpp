/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#pragma once

#include <vector>
#include <utility>


// BaseModel
//   A `BaseModel` interface for a learned model that can be used to
//   predict the CDF of a key, given a training data set to learn from.
template <typename Key>
struct BaseModel {
    // A list of training data representation as <key, eCDF> pairs.
    typedef std::vector<std::pair<Key, double>> TrainingData;

    // predict(key)
    //   Given an input key, returns the CDF of the key, based on the specified
    //   model.
    virtual double predict(Key key) = 0;
};
