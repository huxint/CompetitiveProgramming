#pragma once

#include <bits/stdc++.h>
#include "MillerRabin.hpp"

namespace PollardRho {
    using u32 = unsigned;
    using u64 = unsigned long long;
    using u128 = unsigned __int128;
    static constexpr std::size_t C1 = 1;
    static constexpr std::size_t C2 = 2;
    static constexpr std::size_t M = 512;

    namespace gcd_impl {
        constexpr auto gcd_stein_impl(std::unsigned_integral auto x, std::unsigned_integral auto y) -> decltype(auto) {
            if (x == y) {
                return x;
            }
            const auto a = y - x;
            const auto b = x - y;
            const auto s = x < y ? a : b;
            const auto t = x < y ? x : y;
            const u32 n = std::countr_zero(b);
            return gcd_stein_impl(s >> n, t);
        }

        constexpr auto gcd_stein(std::unsigned_integral auto x, std::unsigned_integral auto y) -> decltype(auto) {
            if (x == 0 or y == 0) {
                return x | y;
            }
            const u32 n = std::countr_zero(x);
            const u32 m = std::countr_zero(y);
            return gcd_stein_impl(x >> n, y >> m) << (n < m ? n : m);
        }
    }

    template <std::unsigned_integral size>
    constexpr auto PollardRho(size value) -> size {
        if (~value & 1) {
            return 2;
        }
        constexpr auto digits = std::numeric_limits<size>::digits;
        using next_size = std::conditional_t<(digits < 64), u64, u128>;
        size inverse = size(2) - value;
        for (u32 i = digits <= 64; i < 5; ++i) {
            inverse *= size(2) - value * inverse;
        }
        auto multiply = [&value, &inverse](size lhs, size rhs) -> size {
            const auto res = next_size(lhs) * rhs;
            return value + size(res >> digits) - size((next_size(size(res) * inverse) * value) >> digits);
        };
        size Z1 = 1, Z2 = 2, res = 0;
        auto get = [&]() -> void {
            size z1 = Z1, z2 = Z2;
            for (std::size_t i = M;; i *= 2) {
                size x1 = z1 + value, x2 = z2 + value;
                for (std::size_t j = 0; j < i; j += M) {
                    size y1 = z1, y2 = z2, q1 = 1, q2 = 2;
                    z1 = multiply(z1, z1) + C1, z2 = multiply(z2, z2) + C2;
                    for (std::size_t k = 0; k < M; ++k) {
                        size t1 = x1 - z1, t2 = x2 - z2;
                        q1 = multiply(q1, t1), q2 = multiply(q2, t2);
                        z1 = multiply(z1, z1) + C1, z2 = multiply(z2, z2) + C2;
                    }
                    q1 = multiply(q1, x1 - z1), q2 = multiply(q2, x2 - z2);
                    size g3 = gcd_impl::gcd_stein(value, multiply(q1, q2));
                    if (g3 == 1) {
                        continue;
                    }
                    if (g3 != value) {
                        res = g3;
                        return;
                    }
                    size g1 = gcd_impl::gcd_stein(value, q1);
                    size g2 = gcd_impl::gcd_stein(value, q2);
                    size C = g1 != 1 ? C1 : C2, x = g1 != 1 ? x1 : x2;
                    size z = g1 != 1 ? y1 : y2, g = g1 != 1 ? g1 : g2;
                    if (g == value) {
                        do {
                            z = multiply(z, z) + C;
                            g = gcd_impl::gcd_stein(value, x - z);
                        } while (g == 1);
                    }
                    if (g != value) {
                        res = g;
                        return;
                    }
                    Z1 += 2, Z2 += 2;
                    return;
                }
            }
        };
        while (res == 0) {
            get();
        }
        return res;
    }

    constexpr auto enumerate_prime_factors(std::unsigned_integral auto value, auto &&call) -> void {
        if (value <= 1) {
            return;
        }
        if (MillerRabin(value)) {
            call(value);
            return;
        }
        auto factor{PollardRho(value)};
        enumerate_prime_factors(factor, call);
        enumerate_prime_factors(value / factor, call);
    }

    constexpr auto factorize(std::unsigned_integral auto value) -> decltype(auto) {
        std::vector<std::pair<decltype(value), u32>> res;
        if (~value & 1) {
            u32 bit_zero = std::countr_zero(value);
            res.emplace_back(2, bit_zero);
            value >>= bit_zero;
        }
        enumerate_prime_factors(value, [&](const auto &x) -> void {
            auto find = std::find_if(res.begin(), res.end(), [&](const auto &element) {
                return element.first == x;
            });
            if (find == res.end()) {
                res.emplace_back(x, 1);
            } else {
                ++find->second;
            }
        });
        std::sort(res.begin(), res.end());
        return res;
    }

    constexpr auto enumerate_factors(std::unsigned_integral auto value, auto &&call) -> void {
        auto factorizer = factorize(value);
        auto dfs = [&](auto &&self, u32 index, decltype(value) prod) -> void {
            if (index == factorizer.size()) {
                call(prod);
                return;
            }
            self(self, index + 1, prod);
            for (auto [prime, count] {factorizer[index]}; count--;) {
                self(self, index + 1, prod *= prime);
            }
        };
        dfs(dfs, 0, 1);
    }

    constexpr auto euler_phi(std::unsigned_integral auto value) -> decltype(value) {
        for (const auto &[prime, _] : factorize(value)) {
            value = value / prime * (prime - 1);
        }
        return value;
    }
}