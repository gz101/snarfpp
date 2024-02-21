/*
 * Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
 * See LICENSE in the directory root for terms of use.
 */

#include "models/linear_spline_model.hpp"
#include "bit_array.hpp"


template <typename Key>
struct SNARF {
    // Underlying predictive model.
    LinearSplineModel<Key> _model;
    // Vector of bitsets used to store the underlying location index data.
    std::vector<BitArray> _bitsets;
    // Vector storing the number of keys in each bitset block.
    std::vector<size_t> _keys_per_block;
    // The total number of input keys.
    size_t _num_keys;
    // The scaling factor used to determine the false positive rate.
    size_t _scaling_factor;
    // The number of elements in each block.
    size_t _block_size;
    // The size of each bitset in bits.
    size_t _bitset_size;
    // The total number of blocks.
    size_t _total_blocks;

    // SNARF(input_Keys, bits_per_key, elements_per_block)
    //   Constructor for the SNARF structure initializes the Golomb-coded bit
    //   arrays. Assumes that the input keys are given in sorted order.
    SNARF(
        const std::vector<Key>& input_keys,
        double bits_per_key,
        size_t block_size,
        size_t R
    ) :
        _model(input_keys, R),
        _num_keys(input_keys.size()),
        _block_size(block_size)
    {
        // Check if more than 3 bits per key.
        if (bits_per_key <= 3) {
            throw std::runtime_error("ERROR: Requires >3 bits per key.");
        }

        // Initialize parameters for SNARF.
        double target_FPR = pow(0.5, bits_per_key - 3.0);
        this->_scaling_factor = pow(2, ceil(log2(1.0 / target_FPR)));
        this->_bitset_size = ceil(log2(1.0 / target_FPR));
        this->_total_blocks = ceil(_num_keys * 1.0 / this->_block_size);

        // Build Golomb compressed bit array of key locations.
        std::vector<size_t> locations;
        _set_locations(input_keys, locations);
        _build_blocks(locations);
    }

    // _set_locations(input_keys, locations)
    //   Calculates and sets the bit array locations for input keys based on the
    //   model's predictions. Assumes keys are in sorted order.
    void _set_locations(
        const std::vector<Key>& input_keys, std::vector<size_t>& locations
    ) {
        locations.clear();
        locations.reserve(this->_num_keys);

        // Collect the predicted location of every input key.
        for (const auto& key : input_keys) {
            double cdf = this->_model.predict(key);

            // Scale to the size of the uncompressed bit array.
            size_t location = size_t(
                floor(cdf * this->_num_keys * this->_scaling_factor)
            );
            location = std::min(
                std::max(location, size_t(0)),
                size_t(this->_num_keys * this->_scaling_factor - 1)
            );

            locations.push_back(location);
        }
    }

    // _create_gcs_block(batch, block)
    //   Encodes a batch of key locations into a GCS block within the input bit
    //   array.
    void _create_gcs_block(const std::vector<size_t>& batch, BitArray& block) {
        // Initialize bit block with sufficient size for everything in batch.
        block = BitArray(
            (this->_bitset_size + 1) * batch.size() + this->_block_size
        );

        size_t offset = 0;

        // Write the binary codes for each key continuously.
        for (auto location : batch) {
            block.write_bits(
                offset, location % this->_scaling_factor, this->_bitset_size
            );
            offset += this->_bitset_size;
        }

        // Write the unary codes for each key continuously.
        size_t delta_zero = 0;
        for (size_t location : batch) {
            size_t unary_part = location / this->_scaling_factor;

            // Write the zeros.
            while (delta_zero < unary_part) {
                block.write_bits(offset++, 0, 1);
                ++delta_zero;
            }

            // Write the terminating one for the unary code.
            block.write_bits(offset++, 1, 1);
        }
    }

    // _build_bit_blocks(locations)
    //   Constructs Golomb-coded bit blocks from sorted locations of input keys.
    void _build_blocks(const std::vector<size_t>& locations) {
        std::vector<size_t> batch;  // locations for current block
        size_t batch_count = ceil(locations.size() * 1.0 / this->_block_size);

        // Initialize bitset array and key count vectors.
        size_t index = 0;
        this->_bitsets.resize(this->_total_blocks);
        this->_keys_per_block.resize(batch_count, 0);

        // Fill each block with keys based on their locations
        for (size_t i = 0; i < batch_count; ++i) {
            batch.resize(0);
            size_t lower_bound = i * this->_block_size * this->_scaling_factor;
            size_t upper_bound = (i + 1) * this->_block_size
                * this->_scaling_factor;

            // Collect locations that fall within the current block's range.
            while (
                index < locations.size() &&
                lower_bound <= locations[index] &&
                locations[index] < upper_bound
            ) {
                // Adjust location relative to start of the block.
                batch.push_back(locations[index++] - lower_bound);
            }

            // Create Golomb-coded bit block for the current batch of locations.
            _create_gcs_block(batch, this->_bitsets[i]);
            // Record the number of keys encoded in the current block.
            this->_keys_per_block[i] = batch.size();
        }
    }

    // _query_block(lower_location, upper_location, bitset, num_keys_read)
    //   Checks if a specific block contains any key within the specified range
    //   [lower_location, upper_location].
    bool _range_query_in_block(
        size_t lower_location,
        size_t upper_location,
        BitArray& bitset,
        size_t num_keys_read
    ) {
        size_t offset_binary = 0;
        size_t offset_unary = num_keys_read * this->_bitset_size;
        size_t delta_zero = 0;

        // Iterate over every key (at most num_keys_read) in the block.
        for (size_t i = 0; i < num_keys_read; ++i) {
            // Get the unary bit.
            size_t unary_part = bitset.read_bit(offset_unary++);

            // Check if at the end of the unary bit for key (i.e. == 1).
            if (
                unary_part &&
                ((delta_zero + 1) * this->_scaling_factor >= lower_location) &&
                (upper_location >= delta_zero * this->_scaling_factor)
            ) {
                // Reconstruct the original location value.
                size_t value = delta_zero * this->_scaling_factor
                    + bitset.read_bits(offset_binary, this->_bitset_size);

                // Check if the location is between the range query.
                if (value >= lower_location && value <= upper_location) {
                    return true;
                }
            }

            delta_zero += (1 - unary_part); // update number of '0's seen.
            i -= (1 - unary_part);  // determine if need to loop to next '0'.
            offset_binary += (unary_part * this->_bitset_size);
        }

        return false;   // no key locations found within this range
    }

    // range_query(lower, upper)
    //   Performs a range query to check if any key within the specified range
    //   [lower, upper] exists.
    bool range_query(const Key& lower, const Key& upper) {
        // Calculate the approximate locations for the query range.
        double lower_cdf = this->_model.predict(lower);
        double upper_cdf = this->_model.predict(upper);

        size_t lower_location = floor(
            lower_cdf * this->_num_keys * this->_scaling_factor
        );
        size_t upper_location = floor(
            upper_cdf * this->_num_keys * this->_scaling_factor
        );

        // Ensure the locations are within bounds.
        lower_location = std::min(
            std::max(lower_location, size_t(0)),
            this->_num_keys * this->_scaling_factor - 1
        );
        upper_location = std::min(
            std::max(upper_location, size_t(0)),
            this->_num_keys * this->_scaling_factor - 1
        );

        // Determine block indices for the lower and upper query locations.
        size_t lower_block_index = lower_location / (
            this->_block_size * this->_scaling_factor
        );
        size_t upper_block_index = upper_location / (
            this->_block_size * this->_scaling_factor
        );

        // If the query range spans multiple blocks, check each relevant block.
        for (
            size_t block_index = lower_block_index;
            block_index <= upper_block_index;
            ++block_index
        ) {
            size_t block_lower_value = (block_index == lower_block_index)
                ? lower_location % (this->_block_size * this->_scaling_factor)
                : 0;
            size_t block_upper_value = (block_index == upper_block_index)
                ? upper_location % (this->_block_size * this->_scaling_factor)
                : this->_block_size * this->_scaling_factor - 1;

            // Adjust block query range to be relative to the current block.
            if (
                _range_query_in_block(
                    block_lower_value,
                    block_upper_value,
                    this->_bitsets[block_index],
                    this->_keys_per_block[block_index]
                )
            ) {
                return true;    // found matching value within range
            }
        }

        return false;   // no matching key found within range
    }

    // size_bytes()
    //   Returns the total size of the SNARF instance.
    size_t size_bytes() {
        size_t size = 0;

        // Add model size.
        size += this->_model.size_bytes();

        // Add member variable sizes.
        size += sizeof(this->_num_keys);
        size += sizeof(this->_scaling_factor);
        size += sizeof(this->_block_size);
        size += sizeof(this->_bitset_size);
        size += sizeof(this->_total_blocks);

        // Add size of key counts.
        for (
            auto it = this->_keys_per_block.begin();
            it != this->_keys_per_block.end();
            ++it
        ) {
            size += sizeof(*it);
        }

        // Add size of each bitset.
        for (
            auto it = this->_bitsets.begin(); it != this->_bitsets.end(); ++it
        ) {
            size += it->size_bytes();
        }

        return size;
    }
};
