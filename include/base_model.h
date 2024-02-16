/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#pragma once

#include <vector>
#include <utility>


// training set representation
typedef std::vector<std::pair<long, long>> TrainingSet;


struct BaseModel {
    // BaseModel(X, Y)
    //   Class constructor that takes a list of tuples as a training set and
    //   returns a trained model as specified.
    BaseModel(TrainingSet& training_set);

    // predict(X)
    //   Given an input key X, returns the CDF of the key, based on the
    //   specified model.
    double predict(long X);
};
