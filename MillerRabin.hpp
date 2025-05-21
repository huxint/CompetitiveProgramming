#pragma once
#include <bits/stdc++.h>

template <std::unsigned_integral size>
constexpr auto MillerRabin(size value) -> bool {
    using u32 = unsigned;
    using u64 = unsigned long long;
    using u128 = unsigned __int128;
    constexpr auto digits = std::numeric_limits<size>::digits;
    using next_size = std::conditional_t<(digits < 64), u64, u128>;

    if (value < 64) {
        return 0x28208a20a08a28ac >> value & 1;
    }

    if (value % 2 == 0 or value % 3 == 0 or value % 5 == 0 or value % 7 == 0 or value % 11 == 0) {
        return false;
    }

    u32 count = std::countr_zero(value - 1);
    size u = (value - 1) >> count, inverse = size(2) - value;

    for (u32 i = digits <= 64; i < 5; ++i) {
        inverse *= size(2) - value * inverse;
    }

    auto multiply = [&value, &inverse](size lhs, size rhs) -> size {
        const auto res = next_size(lhs) * rhs;
        return value + size(res >> digits) - size((next_size(size(res) * inverse) * value) >> digits);
    };

    const size one = -value % value, value_inverse = -next_size(value) % value, value_reduce_one = value - one;

    auto prime_test = [&](std::initializer_list<size> test_base) -> bool {
        return std::all_of(test_base.begin(), test_base.end(), [&](auto base) -> bool {
            if (base >= value) {
                return true;
            }
            size res = one;
            base = multiply(base, value_inverse);
            for (size exp = u; exp != 0; exp >>= 1) {
                if (exp & 1) {
                    res = multiply(res, base);
                }
                base = multiply(base, base);
            }
            if (res = std::min(res, res - value); res == one or res == value_reduce_one) {
                return true;
            }
            for (u32 i = 0; i + 1 < count; ++i) {
                res = multiply(res, res);
                if (auto check = std::min(res, res - value); check == one or check == value_reduce_one) {
                    return check == value_reduce_one;
                }
            }
            return false;
        });
    };

    if (value < 4759123141) {
        return prime_test({2, 7, 61});
    } else if (value < 75792980677) {
        return prime_test({2, 379215, 457083754});
    } else if (value < 21652684502221) {
        return prime_test({2, 1215, 34862, 574237825});
    } else {
        return prime_test({2, 325, 9375, 28178, 450775, 9780504, 1795265022});
    }
}