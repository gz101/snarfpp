/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
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
        this->_linear_models_array.resize(model_array_size);

        // build first linear model
        this->_linear_models_array[0] = _calculate_slope_bias(
            std::make_pair(0, 0), this->_key_array[0]
        );

        // build second model and onwards
        for (size_t i = 0; i < this->_key_array.size(); ++i) {
            this->_linear_models_array[i + 1] = _calculate_slope_bias(
                this->_key_array[i], this->_key_array[i + 1]
            );
        }
    }

    // predict(Key key)
    //   Implements the `BaseModel`'s predict() function that takes an input key
    //   and estimates its CDF.
    double predict(Key key) override {
        SlopeBiasPair model = _linear_models_array[
            this->binary_search(key)
        ];
        double ecdf = model.first * key + model.second;
        return ecdf < 0.0 ? 0.0 : (ecdf > 1.0 ? 1.0 : ecdf);
    }

    // _calculate_slope_bias(pair_1, pair_2)
    //   A simple calculation of (y2 - y1) / (x2 - x1) to generate the slope and
    //   c = y2 - m * x1 and returns it as a pair.
    SlopeBiasPair _calculate_slope_bias(
        typename BaseModel<Key>::KeyCDFPair pair_1,
        typename BaseModel<Key>::KeyCDFPair pair_2
    ) {
        double slope = (
            (pair_2.second - pair_1.second) / (pair_2.first - pair_1.first)
        );
        double bias = pair_2.second - slope * pair_2.first;
        return std::make_pair(slope, bias);
    }

    // size()
    //   Returns the size of the linear model in bytes.
    size_t size_bytes() {
        size_t model_size = 0;

        // Size contribution of base model.
        size_t KeyCDFPair_size = sizeof(Key) + sizeof(double);
        model_size += KeyCDFPair_size * this->_key_array.size();

        // Size contribution of linear spline model.
        size_t SlopeBiasPair_size = sizeof(double) * 2;
        model_size += SlopeBiasPair_size * this->_linear_models_array.size();

        return model_size;
    }

    // print_model()
    //   Implements a member function to print the linear spline model in human-
    //   readable format for debugging purposes.
    void print_model() override {
        std::cout << "--------------------\n";
        std::cout << "KEY ARRAY [Key, eCDF]\n";
        for (
            auto it = this->_key_array.begin();
            it != this->_key_array.end();
            ++it
        ) {
            std::cout << "[" << it->first << ", " << it->second << "]";
        }

        std::cout << "\nLINEAR ARRAY MODEL [Slope, Bias]\n";
        for (
            auto it = this->_linear_models_array.begin();
            it != this->_linear_models_array.end();
            ++it
        ) {
            std::cout << "[" << it->first << ", " << it->second << "]";
        }
        std::cout << "\n--------------------\n";
    }
};
