#pragma once
#include <cstdint>
#include <concepts>
#include <limits>
#include <array>
#include <vector>

/**
 * @快速幂
 */
auto power(auto base, std::size_t exp) {
    decltype(base) res(1);
    for (; exp != 0; exp >>= 1, base *= base) {
        if (exp & 1) {
            res *= base;
        }
    }
    return res;
}

/**
 * @快速幂模
 */
auto power(std::integral auto base, std::size_t exp, const auto &modular) {
    using usize = std::make_unsigned_t<decltype(base)>;
    using ulsize = std::conditional_t<(std::numeric_limits<usize>::digits < 64), std::uint64_t, __uint128_t>;
    ulsize res = 1;
    for (; exp != 0; exp >>= 1, base = static_cast<ulsize>(base) * base % modular) {
        if (exp & 1) {
            res = res * base % modular;
        }
    }
    return static_cast<usize>(res);
}