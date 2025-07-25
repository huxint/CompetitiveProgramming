/**
 * @brief 快速幂算法
 * @details 提供通用的快速幂实现，支持任何重载了*=运算符的类型
 *          包含整数取模版本，添加了类型判断以防止整型溢出
 * @complexity O(log n), 其中n为指数
 */
#pragma once
#include <cstdint>
#include <concepts>
#include <limits>
auto power(auto base, std::size_t exp) {
    decltype(base) res(1);
    for (; exp != 0; exp >>= 1, base *= base) {
        if (exp & 1) {
            res *= base;
        }
    }
    return res;
}

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