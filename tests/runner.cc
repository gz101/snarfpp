/*
 * Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "tests/test_base_model.cc"


int main() {
    test_empty_input();
    test_single_key();
    test_multiple_keys();

    std::cout << "All BaseModel unit tests passed successfully." << std::endl;
    return 0;
}
