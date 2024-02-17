/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#pragma once

#include <vector>
#include <utility>


// BaseSplineModel
//   A `BaseSplineModel` interface for a spline model that can be used to
//   predict the CDF of a key, given a training data set to learn from.
template <class T>
struct BaseSplineModel {
    // A list of training data representation as <key, eCDF> pairs.
    typedef std::vector<std::pair<T, double>> TrainingSet;

    // BaseSplineModel(X, Y)
    //   Class constructor that takes a list of tuples as a training set and
    //   returns a trained model as specified.
    BaseSplineModel(TrainingSet& training_set);

    // predict(key)
    //   Given an input key, returns the CDF of the key, based on the specified
    //   model.
    double predict(T key);

    // search_spline(key)
    //   Searches for the correct spline to use, given an input key.
    size_t search_spline(T key);
};
