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

/**
 * @光速幂
 */
template <std::unsigned_integral T>
class FastPower {
public:
    FastPower(T base, T mod) : _base(base), _mod(mod) {
        _phi = [](std::size_t value) {
            auto res = value;
            std::size_t zero = std::countr_zero(value);
            res >>= (zero != 0), value >>= zero;
            for (std::uint32_t i = 3; i * i <= value; i += 2) {
                if (value % i != 0) {
                    continue;
                }
                (res /= i) *= i - 1;
                do {
                    value /= i;
                } while (value % i == 0);
            }
            return value == 1 ? res : (res /= value) *= value - 1;
        }(mod);

        _sqrt = std::sqrt(mod);

        _pow[0].resize(_sqrt + 1);
        _pow[1].resize(_sqrt + 1);
        _pow[0][0] = _pow[1][0] = 1;

        for (std::uint32_t i = 1; i <= _sqrt; ++i) {
            _pow[0][i] = _pow[0][i - 1] * _base % _mod;
        }

        for (std::uint32_t i = 1; i <= _sqrt; ++i) {
            _pow[1][i] = _pow[1][i - 1] * _pow[0][_sqrt] % _mod;
        }
    }

    T operator()(std::size_t exp) const {
        exp %= _phi;
        return _pow[0][exp % _sqrt] * _pow[1][exp / _sqrt] % _mod;
    }

private:
    T _base, _mod;
    std::size_t _phi, _sqrt;
    std::array<std::vector<std::conditional_t<(std::numeric_limits<T>::digits < 64), std::uint64_t, __uint128_t>>, 2> _pow;
};