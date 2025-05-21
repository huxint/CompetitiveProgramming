#pragma once

#include <bit>
#include <vector>
#include <string>
#include <cstdint>
#include <climits>
#include <ostream>
#include <stdexcept>
#include <algorithm>
#include <type_traits>

/**
 * @author: huxint
 */
template <std::unsigned_integral Block>
class DynamicBitset {
private:
    using block_type = Block;
    using size_type = std::size_t;

public:
    static constexpr size_t npos = size_type(-1);

private:
    class reference {
    private:
        friend class DynamicBitset;
        DynamicBitset *bitset;
        size_type position;
        constexpr reference(DynamicBitset &_bitset, size_type pos) noexcept : bitset(&_bitset), position(pos) {}

    public:
        constexpr reference &operator=(bool value) noexcept {
            if (value) {
                bitset->set(position);
            } else {
                bitset->reset(position);
            }
            return *this;
        }

        constexpr reference &operator=(const reference &other) noexcept {
            if (other) {
                bitset->set(position);
            } else {
                bitset->reset(position);
            }
            return *this;
        }

        constexpr reference &flip() noexcept {
            bitset->flip(position);
            return *this;
        }

        constexpr operator bool() const noexcept {
            return const_cast<const DynamicBitset<Block> &>(*bitset)[position];
        }

        constexpr bool operator~() const noexcept {
            return !const_cast<const DynamicBitset<Block> &>(*bitset)[position];
        }
    };

public:
    constexpr DynamicBitset() noexcept : bit_size(0), blocks() {}

    constexpr DynamicBitset(std::string_view str) {
        bit_size = str.size();
        blocks.resize(block_count(bit_size));
        for (size_type i = 0; i < bit_size; ++i) {
            if (str[bit_size - i - 1] == '1') {
                set(i);
            }
        }
    }

    explicit constexpr DynamicBitset(size_type bit_count, bool value = false) : bit_size(bit_count), blocks(block_count(bit_count), value ? ~Block(0) : Block(0)) {
        sanitize();
    }

    constexpr void resize(size_type bit_count, bool value = false) {
        const size_type old_block_count = blocks.size();
        const size_type new_block_count = block_count(bit_count);
        blocks.resize(new_block_count, value ? ~Block(0) : Block(0));
        if (value && old_block_count < new_block_count) {
            const size_type old_size = bit_size;
            for (size_type i = old_size; i < std::min(bit_count, old_block_count * digits); ++i) {
                set(i);
            }
        }
        bit_size = bit_count;
        sanitize();
    }

    constexpr void clear() {
        blocks.clear();
        bit_size = 0;
    }

    constexpr size_type size() const noexcept {
        return bit_size;
    }

    constexpr bool empty() const noexcept {
        return bit_size == 0;
    }

    constexpr bool at(size_type pos) const {
        if (pos >= bit_size) {
            throw std::out_of_range("DynamicBitset::at: position out of range");
        }
        return (*this)[pos];
    }

    constexpr bool test(size_type pos) const {
        if (pos >= bit_size) {
            throw std::out_of_range("DynamicBitset::test: position out of range");
        }
        return (*this)[pos];
    }

    constexpr void push_back(bool value) {
        const size_type new_size = bit_size + 1;
        const size_type required_blocks = block_count(new_size);
        if (required_blocks > blocks.size()) {
            blocks.resize(required_blocks, Block(0));
        }
        bit_size = new_size;
        value ? set(bit_size - 1) : reset(bit_size - 1);
    }

    constexpr void pop_back() {
        if (bit_size == 0) {
            throw std::out_of_range("DynamicBitset::pop_back: bitset is empty");
        }
        --bit_size;
        const size_type needed_blocks = block_count(bit_size);
        if (blocks.size() > needed_blocks) {
            blocks.resize(needed_blocks);
        }
        sanitize();
    }

    constexpr DynamicBitset &set() {
        for (auto &block : blocks) {
            block = ~Block(0);
        }
        sanitize();
        return *this;
    }

    constexpr DynamicBitset &set(size_type pos) {
        if (pos >= bit_size) {
            throw std::out_of_range("DynamicBitset::set: position out of range");
        }
        blocks[block_index(pos)] |= bit_mask(pos);
        return *this;
    }

    constexpr DynamicBitset &set(size_type first, size_type last) {
        if (first > last || last >= bit_size) {
            throw std::out_of_range("DynamicBitset::set(range): invalid range");
        }
        size_type first_block = block_index(first);
        size_type last_block = block_index(last);
        if (first_block == last_block) {
            Block mask = (~Block(0) << bit_index(first)) & (~Block(0) >> (digits - bit_index(last) - 1));
            blocks[first_block] |= mask;
        } else {
            blocks[first_block] |= ~Block(0) << bit_index(first);
            for (size_type i = first_block + 1; i < last_block; ++i) {
                blocks[i] = ~Block(0);
            }
            blocks[last_block] |= ~Block(0) >> (digits - bit_index(last) - 1);
        }
        return *this;
    }

    constexpr DynamicBitset &reset() {
        for (auto &block : blocks) {
            block = Block(0);
        }
        return *this;
    }

    constexpr DynamicBitset &reset(size_type pos) {
        if (pos >= bit_size) {
            throw std::out_of_range("DynamicBitset::reset: position out of range");
        }
        blocks[block_index(pos)] &= ~bit_mask(pos);
        return *this;
    }

    constexpr DynamicBitset &reset(size_type first, size_type last) {
        if (first > last || last >= bit_size) {
            throw std::out_of_range("DynamicBitset::reset(range): invalid range");
        }
        size_type first_block = block_index(first);
        size_type last_block = block_index(last);
        if (first_block == last_block) {
            Block mask = (~Block(0) << bit_index(first)) & (~Block(0) >> (digits - bit_index(last) - 1));
            blocks[first_block] &= ~mask;
        } else {
            blocks[first_block] &= ~(~Block(0) << bit_index(first));
            for (size_type i = first_block + 1; i < last_block; ++i) {
                blocks[i] = Block(0);
            }
            blocks[last_block] &= ~(~Block(0) >> (digits - bit_index(last) - 1));
        }
        return *this;
    }

    constexpr DynamicBitset &flip() {
        for (auto &block : blocks) {
            block = ~block;
        }
        sanitize();
        return *this;
    }

    constexpr DynamicBitset &flip(size_type pos) {
        if (pos >= bit_size) {
            throw std::out_of_range("DynamicBitset::flip: position out of range");
        }
        blocks[block_index(pos)] ^= bit_mask(pos);
        return *this;
    }

    constexpr DynamicBitset &flip(size_type first, size_type last) {
        if (first > last || last >= bit_size) {
            throw std::out_of_range("DynamicBitset::flip(range): invalid range");
        }
        size_type first_block = block_index(first);
        size_type last_block = block_index(last);
        if (first_block == last_block) {
            Block mask = (~Block(0) << bit_index(first)) & (~Block(0) >> (digits - bit_index(last) - 1));
            blocks[first_block] ^= mask;
        } else {
            blocks[first_block] ^= ~Block(0) << bit_index(first);
            for (size_type i = first_block + 1; i < last_block; ++i) {
                blocks[i] = ~blocks[i];
            }
            blocks[last_block] ^= ~Block(0) >> (digits - bit_index(last) - 1);
        }
        return *this;
    }

    constexpr bool any() const {
        if (empty()) {
            return false;
        }
        const size_type last_block = blocks.size() - 1;
        for (size_type i = 0; i < last_block; ++i) {
            if (blocks[i] != Block(0)) {
                return true;
            }
        }
        return blocks[last_block] != Block(0);
    }

    constexpr bool all() const {
        if (empty()) {
            return true;
        }
        const size_type last_block = blocks.size() - 1;
        for (size_type i = 0; i < last_block; ++i) {
            if (blocks[i] != ~Block(0)) {
                return false;
            }
        }
        const size_type extra_bits = bit_size % digits;
        if (extra_bits == 0) {
            return blocks[last_block] == ~Block(0);
        } else {
            const Block mask = (static_cast<Block>(1) << extra_bits) - 1;
            return (blocks[last_block] & mask) == mask;
        }
    }

    constexpr bool any(size_type first, size_type last) const {
        if (first > last || last >= bit_size) {
            throw std::out_of_range("DynamicBitset::any(range): invalid range");
        }
        size_type first_block = block_index(first);
        size_type last_block = block_index(last);
        if (first_block == last_block) {
            Block mask = (~Block(0) << bit_index(first)) & (~Block(0) >> (digits - bit_index(last) - 1));
            return (blocks[first_block] & mask) != Block(0);
        } else {
            if ((blocks[first_block] & (~Block(0) << bit_index(first))) != Block(0)) {
                return true;
            }
            for (size_type i = first_block + 1; i < last_block; ++i) {
                if (blocks[i] != Block(0)) {
                    return true;
                }
            }
            return (blocks[last_block] & (~Block(0) >> (digits - bit_index(last) - 1))) != Block(0);
        }
    }

    constexpr bool all(size_type first, size_type last) const {
        if (first > last || last >= bit_size) {
            throw std::out_of_range("DynamicBitset::all(range): invalid range");
        }
        size_type first_block = block_index(first);
        size_type last_block = block_index(last);
        if (first_block == last_block) {
            Block mask = (~Block(0) << bit_index(first)) & (~Block(0) >> (digits - bit_index(last) - 1));
            return (blocks[first_block] & mask) == mask;
        } else {
            Block first_mask = ~Block(0) << bit_index(first);
            if ((blocks[first_block] & first_mask) != first_mask) {
                return false;
            }
            for (size_type i = first_block + 1; i < last_block; ++i) {
                if (blocks[i] != ~Block(0)) {
                    return false;
                }
            }
            Block last_mask = ~Block(0) >> (digits - bit_index(last) - 1);
            return (blocks[last_block] & last_mask) == last_mask;
        }
    }

    constexpr bool none() const {
        return !any();
    }

    constexpr size_type count() const {
        size_type res = 0;
        for (const auto &block : blocks) {
            res += std::popcount(block);
        }
        return res;
    }

    constexpr size_type count(size_type first, size_type last) const {
        if (first > last || last >= bit_size) {
            throw std::out_of_range("DynamicBitset::count(range): invalid range");
        }
        size_type first_block = block_index(first);
        size_type last_block = block_index(last);
        if (first_block == last_block) {
            Block mask = (~Block(0) << bit_index(first)) & (~Block(0) >> (digits - bit_index(last) - 1));
            return std::popcount(blocks[first_block] & mask);
        } else {
            size_type res = std::popcount(blocks[first_block] & (~Block(0) << bit_index(first)));
            for (size_type i = first_block + 1; i < last_block; ++i) {
                res += std::popcount(blocks[i]);
            }
            res += std::popcount(blocks[last_block] & (~Block(0) >> (digits - bit_index(last) - 1)));
            return res;
        }
    }

    constexpr std::string to_string() const {
        std::string res;
        res.reserve(bit_size);
        for (size_type i = bit_size; i > 0; --i) {
            res.push_back((*this)[i - 1] ? '1' : '0');
        }
        return res;
    }

    constexpr size_type find_first() const {
        for (size_type i = 0; i < blocks.size(); ++i) {
            if (blocks[i] != Block(0)) {
                return i * digits + std::countr_zero(blocks[i]);
            }
        }
        return npos;
    }

    constexpr size_type find_next(size_type pos) const {
        if (pos >= bit_size) {
            return npos;
        }
        size_type block_pos = block_index(pos);
        Block mask = ~((static_cast<Block>(1) << bit_index(pos)) - 1);
        mask &= ~bit_mask(pos);
        Block current = blocks[block_pos] & mask;
        if (current != Block(0)) {
            return block_pos * digits + std::countr_zero(current);
        }
        for (++block_pos; block_pos < blocks.size(); ++block_pos) {
            if (blocks[block_pos] != Block(0)) {
                return block_pos * digits + std::countr_zero(blocks[block_pos]);
            }
        }
        return npos;
    }

    constexpr size_type find_first_zero() const {
        for (size_type i = 0; i < blocks.size(); ++i) {
            if (~blocks[i] != Block(0)) {
                size_type pos = i * digits + std::countr_zero(~blocks[i]);
                return pos < bit_size ? pos : npos;
            }
        }
        return npos;
    }

    constexpr size_type find_next_zero(size_type pos) const {
        if (pos >= bit_size) {
            return npos;
        }
        size_type block_pos = block_index(pos);
        Block mask = ~((static_cast<Block>(1) << bit_index(pos)) - 1);
        mask &= ~bit_mask(pos);
        Block current = ~blocks[block_pos] & mask;
        if (current != Block(0)) {
            size_type res = block_pos * digits + std::countr_zero(current);
            return res < bit_size ? res : npos;
        }
        for (++block_pos; block_pos < blocks.size(); ++block_pos) {
            if (~blocks[block_pos] != Block(0)) {
                size_type res = block_pos * digits + std::countr_zero(~blocks[block_pos]);
                return res < bit_size ? res : npos;
            }
        }
        return npos;
    }

    constexpr bool is_subset_of(const DynamicBitset &other) const {
        if (bit_size > other.bit_size) {
            return false;
        }
        const size_type min_blocks = std::min(blocks.size(), other.blocks.size());
        for (size_type i = 0; i < min_blocks; ++i) {
            if ((blocks[i] & ~other.blocks[i]) != Block(0)) {
                return false;
            }
        }
        for (size_type i = min_blocks; i < blocks.size(); ++i) {
            if (blocks[i] != Block(0)) {
                return false;
            }
        }
        return true;
    }

    constexpr bool intersects(const DynamicBitset &other) const {
        const size_type min_blocks = std::min(blocks.size(), other.blocks.size());
        for (size_type i = 0; i < min_blocks; ++i) {
            if ((blocks[i] & other.blocks[i]) != Block(0)) {
                return true;
            }
        }
        return false;
    }

    constexpr DynamicBitset &rotate_left(size_type n) {
        if (empty() || n % bit_size == 0) {
            return *this;
        }
        n %= bit_size;
        DynamicBitset temp(*this);
        *this <<= n;
        for (size_type i = 0; i < n; ++i) {
            if (temp[bit_size - n + i]) {
                set(i);
            }
        }
        return *this;
    }

    constexpr DynamicBitset &rotate_right(size_type n) {
        if (empty() || n % bit_size == 0) {
            return *this;
        }
        n %= bit_size;
        DynamicBitset temp(*this);
        *this >>= n;
        for (size_type i = 0; i < n; ++i) {
            if (temp[i]) {
                set(bit_size - n + i);
            }
        }
        return *this;
    }

    constexpr DynamicBitset get_subset(size_type start, size_type length) const {
        if (start >= bit_size || length == 0) {
            return DynamicBitset();
        }
        length = std::min(length, bit_size - start);
        DynamicBitset res(length);
        for (size_type i = 0; i < length; ++i) {
            if ((*this)[start + i]) {
                res.set(i);
            }
        }
        return res;
    }

    constexpr void for_each_set_bit(auto &&call) const {
        for (size_type i = 0; i < blocks.size(); ++i) {
            Block block = blocks[i];
            while (block) {
                size_type bit_pos = std::countr_zero(block);
                size_type global_pos = i * digits + bit_pos;
                if (global_pos < bit_size) {
                    call(global_pos);
                }
                block &= ~(Block(1) << bit_pos);
            }
        }
    }

    constexpr void for_each_unset_bit(auto &&call) const {
        for (size_type i = 0; i < blocks.size(); ++i) {
            Block block = ~blocks[i];
            if (i == blocks.size() - 1) {
                const size_type extra_bits = bit_size % digits;
                if (extra_bits > 0) {
                    block &= (Block(1) << extra_bits) - 1;
                }
            }
            while (block) {
                size_type bit_pos = std::countr_zero(block);
                size_type global_pos = i * digits + bit_pos;
                if (global_pos < bit_size) {
                    call(global_pos);
                }
                block &= ~(Block(1) << bit_pos);
            }
        }
    }

    constexpr reference operator[](size_type pos) {
        return reference(*this, pos);
    }

    [[nodiscard]] constexpr bool operator[](size_type pos) const {
        return (blocks[block_index(pos)] & bit_mask(pos)) != 0;
    }

    constexpr DynamicBitset &operator&=(const DynamicBitset &rhs) {
        const size_type min_blocks = std::min(blocks.size(), rhs.blocks.size());
        for (size_type i = 0; i < min_blocks; ++i) {
            blocks[i] &= rhs.blocks[i];
        }
        for (size_type i = min_blocks; i < blocks.size(); ++i) {
            blocks[i] = Block(0);
        }
        return *this;
    }

    constexpr DynamicBitset &operator|=(const DynamicBitset &rhs) {
        const size_type min_blocks = std::min(blocks.size(), rhs.blocks.size());
        for (size_type i = 0; i < min_blocks; ++i) {
            blocks[i] |= rhs.blocks[i];
        }
        return *this;
    }

    constexpr DynamicBitset &operator^=(const DynamicBitset &rhs) {
        const size_type min_blocks = std::min(blocks.size(), rhs.blocks.size());
        for (size_type i = 0; i < min_blocks; ++i) {
            blocks[i] ^= rhs.blocks[i];
        }
        return *this;
    }

    constexpr DynamicBitset &operator<<=(size_type n) {
        if (n >= bit_size) {
            reset();
            return *this;
        }
        if (n > 0) {
            const size_type block_shift = n / digits;
            const size_type bit_shift = n % digits;
            if (bit_shift == 0) {
                for (size_type i = blocks.size(); i > block_shift; --i) {
                    blocks[i - 1] = blocks[i - block_shift - 1];
                }
            } else {
                const size_type reverse_bit_shift = digits - bit_shift;
                for (size_type i = blocks.size(); i > block_shift + 1; --i) {
                    blocks[i - 1] = (blocks[i - block_shift - 1] << bit_shift) | (blocks[i - block_shift - 2] >> reverse_bit_shift);
                }
                if (block_shift + 1 < blocks.size()) {
                    blocks[block_shift] = blocks[0] << bit_shift;
                }
            }
            for (size_type i = 0; i < block_shift && i < blocks.size(); ++i) {
                blocks[i] = Block(0);
            }
            sanitize();
        }
        return *this;
    }

    constexpr DynamicBitset &operator>>=(size_type n) {
        if (n >= bit_size) {
            reset();
            return *this;
        }
        if (n > 0) {
            const size_type block_shift = n / digits;
            const size_type bit_shift = n % digits;
            if (bit_shift == 0) {
                for (size_type i = 0; i < blocks.size() - block_shift; ++i) {
                    blocks[i] = blocks[i + block_shift];
                }
            } else {
                const size_type reverse_bit_shift = digits - bit_shift;
                for (size_type i = 0; i < blocks.size() - block_shift - 1; ++i) {
                    blocks[i] = (blocks[i + block_shift] >> bit_shift) | (blocks[i + block_shift + 1] << reverse_bit_shift);
                }
                if (block_shift < blocks.size()) {
                    blocks[blocks.size() - block_shift - 1] = blocks[blocks.size() - 1] >> bit_shift;
                }
            }
            for (size_type i = blocks.size() - block_shift; i < blocks.size(); ++i) {
                blocks[i] = Block(0);
            }
        }
        return *this;
    }

    friend constexpr DynamicBitset operator~(const DynamicBitset &value) {
        return DynamicBitset(value).flip();
    }

    friend constexpr DynamicBitset operator&(const DynamicBitset &lhs, const DynamicBitset &rhs) {
        return DynamicBitset(lhs) &= rhs;
    }

    friend constexpr DynamicBitset operator|(const DynamicBitset &lhs, const DynamicBitset &rhs) {
        return DynamicBitset(lhs) |= rhs;
    }

    friend constexpr DynamicBitset operator^(const DynamicBitset &lhs, const DynamicBitset &rhs) {
        return DynamicBitset(lhs) ^= rhs;
    }

    friend constexpr DynamicBitset operator<<(const DynamicBitset &value, size_type n) {
        return DynamicBitset(value) <<= n;
    }

    friend constexpr DynamicBitset operator>>(const DynamicBitset &value, size_type n) {
        return DynamicBitset(value) >>= n;
    }

    friend constexpr bool operator==(const DynamicBitset &lhs, const DynamicBitset &rhs) {
        if (lhs.bit_size != rhs.bit_size) {
            return false;
        }
        const size_type min_blocks = std::min(lhs.blocks.size(), rhs.blocks.size());
        for (size_type i = 0; i < min_blocks; ++i) {
            if (lhs.blocks[i] != rhs.blocks[i]) {
                return false;
            }
        }
        return true;
    }

    template <typename Ostream>
    friend Ostream &operator<<(Ostream &ostream, const DynamicBitset &self) {
        return ostream << self.to_string();
    }

private:
    std::size_t bit_size;
    std::vector<Block> blocks;
    static constexpr std::size_t digits = std::numeric_limits<Block>::digits;
    static constexpr size_t block_count(std::size_t bit_count) {
        return (bit_count + digits - 1) / digits;
    }

    static constexpr size_t block_index(std::size_t pos) {
        return pos / digits;
    }

    static constexpr size_t bit_index(std::size_t pos) {
        return pos % digits;
    }

    static constexpr Block bit_mask(std::size_t pos) {
        return static_cast<Block>(1) << bit_index(pos);
    }

    constexpr void sanitize() {
        if (bit_size > 0) {
            const std::size_t extra_bits = bit_size % digits;
            if (extra_bits > 0) {
                const Block mask = (static_cast<Block>(1) << extra_bits) - 1;
                blocks.back() &= mask;
            }
        }
    }
};
using dynamic_bitset = DynamicBitset<std::uint64_t>;