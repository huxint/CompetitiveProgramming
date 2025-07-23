#pragma once
#include <cstdint>
#include <concepts>
#include <limits>

/*
简单的快速幂
第一个支持重载了 *= 运算符的任何类
第二个就是整数取模，加了一些类型的判断，防止整形溢出的错误
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