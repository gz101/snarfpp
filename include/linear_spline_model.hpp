/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#pragma once

#include "base_spline_model.hpp"


// LinearSplineModel
//   A `LinearSplineModel` interface for a linear spline model that can be used
//   to build the array of linear splines and implement the `predict` function.
template <typename Key>
struct LinearSplineModel : BaseSplineModel<Key> {
    // Data representation of a single linear model as a <slope, bias> pair.
    typedef std::pair<double, double> SlopeBiasPair;

    // An array of linear models (of type `SlopeBiasPair`).
    std::vector<SlopeBiasPair> _linear_models_array;

    // LinearSplineModel(input_keys, R)
    //   Constructs a spline of linear models using an array of `SlopeBiasPair`s
    //   given the selected key array.
    LinearSplineModel(
        const std::vector<Key>& input_keys, size_t R
    ) : BaseSplineModel<Key>(input_keys, R) {
        size_t model_array_size = this->_key_array.size() + 1;
        _linear_models_array.resize(model_array_size);

        // build first linear model
        _linear_models_array[0] = _calculate_slope_bias(
            std::make_pair(0, 0), this->_key_array[0]
        );

        // build second model and onwards
        for (size_t i = 0; i < model_array_size - 1; ++i) {
            _linear_models_array[i] = _calculate_slope_bias(
                this->_key_array[i], this->_key_array[i + 1]
            );
        }
    }

    // predict(Key key)
    //   Implements the `BaseModel`'s predict() function that takes an input key
    //   and estimates its CDF.
    double predict(Key key) {
        return 0.0;
    }

    // _calculate_slope_bias(pair_1, pair_2)
    //   A simple calculation of (y2 - y1) / (x2 - x1) to generate the slope and
    //   c = y2 - m * x1 and returns it as a pair.
    SlopeBiasPair _calculate_slope_bias(
        BaseModel<Key>::KeyCDFPair pair_1, BaseModel<Key>::KeyCDFPair pair_2
    ) {
        double slope = (
            (pair_2.second - pair_1.second) / (pair_2.first - pair_1.first)
        );
        double bias = pair2.second - slope * pair2.first;
        return std::make_pair(slope, bias);
    }
};
