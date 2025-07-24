/**
 * @brief Miller-Rabin素性测试算法
 * @details 高效的概率性素数判断算法，适用于判断大整数是否为素数
 *          对于64位以内的数具有确定性结果
 * @author huxint
 * @complexity O(k log^3 n), 其中k为测试轮数，n为待测数
 */
#pragma once
#include <cstdint>
#include <concepts>
#include <limits>
#include <algorithm>
template <std::unsigned_integral T>
constexpr bool MillerRabin(T value) {
    using u32 = std::uint32_t;
    constexpr auto digits = std::numeric_limits<T>::digits;
    using long_size = std::conditional_t<(digits < 64), std::uint64_t, __uint128_t>;

    if (value < 64) {
        return 0x28208a20a08a28ac >> value & 1;
    }

    if (value % 2 == 0 or value % 3 == 0 or value % 5 == 0 or value % 7 == 0 or value % 11 == 0) {
        return false;
    }

    u32 count = std::countr_zero(value - 1);
    T u = (value - 1) >> count, inverse = T(2) - value;

    for (u32 i = digits <= 64; i < 5; ++i) {
        inverse *= T(2) - value * inverse;
    }

    auto multiply = [&value, &inverse](T lhs, T rhs) -> T {
        const auto res = long_size(lhs) * rhs;
        return value + T(res >> digits) - T((long_size(T(res) * inverse) * value) >> digits);
    };

    const T one = -value % value, value_inverse = -long_size(value) % value, value_reduce_one = value - one;

    auto prime_test = [&](std::initializer_list<T> test_base) -> bool {
        return std::all_of(test_base.begin(), test_base.end(), [&](auto base) -> bool {
            if (base >= value) {
                return true;
            }
            T res = one;
            base = multiply(base, value_inverse);
            for (T exp = u; exp != 0; exp >>= 1) {
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