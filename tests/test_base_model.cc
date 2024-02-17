/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include <cassert>
#include <iostream>

#include "../include/base_model.h"


template <typename Key>
struct MockModel : public BaseModel<Key> {
    // constructor that forwards the vector of keys to the BaseModel, with a
    // default argument
    MockModel(const std::vector<Key>& keys = {}) : BaseModel<Key>(keys) {}

    double predict(Key key) override {
        return key * 1.0;
    }
};


// test_empty_input()
//   Tests the `BaseModel` with no input keys.
void test_empty_input() {
    MockModel<int> model(std::vector<int>{});
    assert(model.training_data.empty());
}


// test_single_key()
//   Tests the `BaseModel` correctly constructs an eCDF with a single input key.
void test_single_key() {
    MockModel<int> model({1});
    assert(model.training_data.size() == 1);
    assert(model.training_data[0] == std::make_pair(1, 0.0));
}


// test_multiple_keys()
//   Tests the `BaseModel` correctly constructs an eCDF with multiple keys.
void test_multiple_keys() {
    MockModel<int> model({1, 2, 3});
    assert(model.training_data.size() == 3);
    assert(model.training_data[0] == std::make_pair(1, 0.0));
    assert(model.training_data[1] == std::make_pair(2, 1.0 / 3));
    assert(model.training_data[2] == std::make_pair(3, 2.0 / 3));
}


int run() {
    test_empty_input();
    test_single_key();
    test_multiple_keys();

    std::cout << "All BaseModel unit tests passed successfully." << std::endl;
    return 0;
}
