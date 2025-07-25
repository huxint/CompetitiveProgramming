/**
 * @brief 字符串哈希算法
 * @details 多项式滚动哈希实现，支持快速字符串匹配和子串比较
 *          支持多重哈希以降低冲突概率，可配合MillerRabin生成编译期随机质数模数
 *          支持字符串和数字容器的哈希计算
 * @author huxint
 * @complexity 预处理: O(n), 查询: O(1)
 */
#pragma once
#include <array>
#include <limits>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <string_view>
template <std::size_t width, std::array<std::uint64_t, width> base, std::array<std::uint64_t, width> modular>
class StringHash {
private:
    using u128 = unsigned __int128;
    using hash = std::array<std::uint64_t, width>;

public:
    StringHash() {}

    StringHash(std::string_view str) : StringHash(str.begin(), str.end()) {}

    template <typename Iterator>
    StringHash(Iterator begin, Iterator end) : _size(end - begin), power(size() + 1), table(size() + 1) {
        std::fill(power[0].begin(), power[0].end(), 1);
        std::fill(table[0].begin(), table[0].end(), 0);
        for (std::size_t i = 0; i < size(); ++i) {
            for (std::size_t j = 0; j < width; ++j) {
                power[i + 1][j] = multiply(power[i][j], base[j], j);
                table[i + 1][j] = multiply(table[i][j], base[j], j) + *begin++;
                if (table[i + 1][j] >= modular[j]) {
                    table[i + 1][j] -= modular[j];
                }
            }
        }
    }

    std::size_t size() const {
        return _size;
    }

    hash query() const {
        return table[size()];
    }

    hash query(std::size_t l, std::size_t r) const {
        hash res{};
        for (std::size_t i = 0; i < width; ++i) {
            res[i] = table[r + 1][i] - multiply(table[l][i], power[r - l + 1][i], i);
            if (res[i] >= modular[i]) {
                res[i] += modular[i];
            }
        }
        return res;
    }

    friend bool operator==(const StringHash &lhs, const StringHash &rhs) {
        return lhs.query() == rhs.query();
    }

    friend std::strong_ordering operator<=>(const StringHash &lhs, const StringHash &rhs) {
        return lhs.query() <=> rhs.query();
    }

    template <typename Ostream>
    friend Ostream &operator<<(Ostream &ostream, const StringHash &self) {
        return ostream << self.query();
    }

private:
    std::size_t _size;
    std::vector<hash> power;
    std::vector<hash> table;

    std::int64_t multiply(std::uint64_t lhs, std::uint64_t rhs, std::size_t i) const {
        std::int64_t res = lhs * rhs;
        res -= static_cast<std::uint64_t>(static_cast<long double>(lhs) * rhs / modular[i]) * modular[i];
        return res < 0 ? res + modular[i] : res >= modular[i] ? res - modular[i] : res;
    }
};

namespace RandomHashing {
    constexpr std::uint64_t seed() {
        std::uint64_t shifted = 0;
        for (const auto c : __TIME__ __TIMESTAMP__ __DATE__) {
            (shifted += c) *= 6364136223846793005ULL;
            shifted ^= shifted << 13;
            shifted ^= shifted >> 7;
            shifted ^= shifted << 17;
        }
        return shifted;
    }

#include "../Math/MillerRabin.hpp"

    constexpr std::size_t generate_prime(std::size_t left, std::size_t right) {
        auto shifted = seed();
        std::size_t value;
        do {
            shifted ^= shifted << 13;
            shifted ^= shifted >> 7;
            shifted ^= shifted << 17;
            value = shifted % (right - left) + left;
        } while (not MillerRabin(value));
        return value;
    }
}

constexpr std::uint64_t base = RandomHashing::generate_prime(100000000000ULL, 10000000000000ULL);
constexpr std::uint64_t modular = RandomHashing::generate_prime(100000000000000000ULL, 3000000000000000000ULL);
constexpr std::array<std::uint64_t, 1> Base = {base};
constexpr std::array<std::uint64_t, 1> Modular = {modular};
using Hashing = StringHash<1, Base, Modular>;