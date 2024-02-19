/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include <algorithm>
#include "models/linear_spline_model.hpp"
#include "bit_array.hpp"


template <typename Key>
struct SNARF {
    // Predictive model that can be varied.
    LinearSplineModel<Key> _model;
    // An array of bit sets used to store the underlying data.
    std::vector<BitArray> _bit_set_array;
    // Specifies the number of keys in each block.
    std::vector<size_t> _keys_per_block;
    // The total number of input keys.
    size_t _total_keys;
    // Golomb parameter calculated based on the desired number of keys per bit.
    size_t _golomb_param;
    // The specified input number of elements per block.
    size_t _block_size;
    // The number of bits required for GCS remainder.
    size_t _bit_size;
    // The total number of blocks needed to store all keys.
    size_t _total_blocks;

    // SNARF(input_Keys, bits_per_key, elements_per_block)
    //   Constructor for the SNARF structure initializes the Golomb-coded bit
    //   arrays. Assumes that the input keys are given in sorted order.
    SNARF(
        const std::vector<Key>& input_keys,
        double bits_per_key,
        size_t elements_per_block,
        size_t R
    ) : _model(input_keys, R),
        _total_keys(input_keys.size()),
        _block_size(elements_per_block)
    {
        // Set parameters for SNARF.
        _set_golomb_parameters(bits_per_key);
        this->_total_blocks = ceil(this->_total_keys * 1.0 / this->_block_size);
        this->_bit_set_array.resize(this->_total_blocks);

        // Determine the locations for each key using predictive model.
        std::vector<size_t> locations;
        _set_locations(input_keys, locations);

        // Create Golomb-coded blocks for the determined locations.
        _build_bit_blocks(locations);
    }

    // _set_golomb_parameters(bits_per_key)
    //   Sets the Golomb coding parameters based on the desired average bits
    //   per key.
    void _set_golomb_parameters(double bits_per_key) {
        double target_FPR = pow(0.5, bits_per_key - 3.0);
        this->_golomb_param = pow(2, ceil(log2(1.0 / target_FPR)));
        this->_bit_size = ceil(log2(1.0 / target_FPR));
    }

    // _set_locations(input_keys, locations)
    //   Calculates and sets the bit array locations for input keys based on the
    //   model's predictions. Assumes keys are in sorted order.
    void _set_locations(
        const std::vector<Key>& input_keys, std::vector<size_t>& locations
    ) {
        for (const Key& key : input_keys) {
            double cdf = this->_model.predict(key);

            // Scale to size of the un-compressed bit array.
            size_t location = floor(
                cdf * this->_total_keys * this->_golomb_param
            );
            location = std::max(
                size_t(0ULL),
                std::min(
                    location,
                    size_t(this->_total_keys * this->_golomb_param - 1)
                )
            );

            locations.push_back(location);
        }

        // If input keys are not in sorted order, use this.
        // std::sort(locations.begin(), locations.end());
    }

    // _build_bit_blocks(locations)
    //   Constructs Golomb-coded bit blocks from sorted locations of input keys.
    void _build_bit_blocks(const std::vector<size_t>& locations) {
        std::vector<size_t> current_batch;  // locations for current block

        // Fill each block with keys based on their locations
        for (size_t i = 0, start = 0; i < this->_total_blocks; ++i) {
            current_batch.clear();

            size_t lo = i * this->_block_size * this->_golomb_param;
            size_t hi = (i + 1) * this->_block_size * this->_golomb_param;

            // Collect locations that fall within the current block's range.
            for (; start < locations.size() && locations[start] < hi; ++start) {
                if (locations[start] >= lo) {
                    // Adjust location relative to start of the block.
                    current_batch.push_back(locations[start] - lo);
                }
            }

            // Create Golomb-coded bit block for the current batch of locations.
            _create_gcs_block(current_batch, this->_bit_set_array[i]);
            // Record the number of keys encoded in the current block.
            this->_keys_per_block.push_back(current_batch.size());
        }
    }

    // _create_gcs_block(current_batch, bit_set)
    //   Encodes a batch of keys into a GCS block within the input bit array.
    void _create_gcs_block(
        const std::vector<size_t>& current_batch, BitArray& bit_set
    ) {
        // Initialize bit block with sufficient size for everything in batch.
        bit_set = BitArray(
            (this->_bit_size + 1) * current_batch.size() + this->_block_size
        );

        size_t offset = 0;

        // Iterate over every key location in current batch for encoding.
        for (auto value : current_batch) {
            size_t quotient = value / this->_golomb_param;
            size_t remainder = value % this->_golomb_param;

            // Write unary code for quotient.
            for (size_t j = 0; j < quotient; ++j) {
                bit_set.write_bits(offset++, 0, 1);
            }
            bit_set.write_bits(offset++, 1, 1); // delimiter

            // Write binary code for remainder.
            bit_set.write_bits(offset, remainder, this->_bit_size);
            offset += this->_bit_size;
        }
    }

    // range_query(lo, hi)
    //   Performs a range query to check if any key within the specified range
    //   [lo, hi] exists.
    bool range_query(Key lo, Key hi) {
        // Obtain array positions for the high and low ranges of the query.
        double cdf_lo = this->_model.predict(lo);
        double cdf_hi = this->_model.predict(hi);
        size_t lo_pos = floor(cdf_lo * this->_total_keys * this->_golomb_param);
        size_t hi_pos = floor(cdf_hi * this->_total_keys * this->_golomb_param);

        // Determine the block indices where the lower and upper positions are.
        size_t lo_block_index = lo_pos / (
            this->_block_size * this->_golomb_param
        );
        size_t hi_block_index = hi_pos / (
            this->_block_size * this->_golomb_param
        );

        // Iterate over each block that could contain the query range.
        for (
            size_t i = lo_block_index;
            i <= hi_block_index && i < this->_total_blocks;
            ++i
        ) {
            // Start of current block in the GCS.
            size_t block_start = i * this->_block_size * this->_golomb_param;

            // Adjust lower and higher positions relative to the start of the
            // current block.
            size_t relative_lo_pos = lo_pos > block_start
                ? lo_pos - block_start : 0;
            size_t relative_hi_pos =
                hi_pos < (block_start + this->_block_size * this->_golomb_param)
                ? hi_pos - block_start
                : this->_block_size * this->_golomb_param - 1;

            // Check if current block contains any key within the query range.
            if (
                _query_block(
                    this->_bit_set_array[i],
                    relative_lo_pos,
                    relative_hi_pos,
                    this->_keys_per_block[i]
                )
            ) {
                return true;
            }
        }

        return false;   // no key found in the range
    }

    // _query_block(bit_set, lo, hi, num_keys_read)
    //   Checks if a specific block contains any key within the specified range
    //   [lo, hi].
    bool _query_block(BitArray& bit_set, Key lo, Key hi, size_t num_keys_read) {
        size_t offset = 0;

        // Reconstruct every key in block until the value is found.
        for (size_t i = 0; i < num_keys_read; ++i) {
            size_t quotient = 0;

            // Reading unary part for quotient.
            while (!bit_set.read_bit(offset)) {
                ++quotient;
                ++offset;
            }
            ++offset;   // skip the delimiter '1' bit at end of unary code

            // Reading binary part for remainder.
            size_t remainder = bit_set.read_bits(offset, this->_bit_size);
            offset += this->_bit_size;

            // Reconstruct value.
            Key value = quotient * this->_golomb_param + remainder;

            // Check if reconstructed key value falls within query range.
            if (value >= lo && value <= hi) {
                return true;
            }
        }

        return false;   // no key values found within range.
    }

    // size()
    //   Returns the total size of the SNARF instance.
    size_t size() {
        return 16;
    }
};
