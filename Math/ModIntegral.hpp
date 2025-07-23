#pragma once
#include <cstdint>
#include <concepts>
#include <type_traits>

/*
取模整数类
如果模数是 32 位范围的，using modint = ModIntegral<998244353U, true>
如果模数是 64 位范围的，using modint = ModIntegral<998244353ULL, false>

用 U 和 ULL 可以让编译器清楚是32位还是64位

后面的布尔值代表 mod 是否是质数，用于让求逆元时使用费马小定理还是扩展欧几里得

mod 必须是编译期已知，比如题面上的1000000007, 998244353等等
而不是根据输入来的 mod

支持取模、取逆、幂运算、比较运算

支持四则运算

支持输入输出

支持隐式转换为 bool
*/
template <std::unsigned_integral auto modular, bool prime>
class ModIntegral {
private:
    using T = std::decay_t<decltype(modular)>;

public:
    constexpr ModIntegral() : _value() {}

    constexpr ModIntegral(std::unsigned_integral auto value) : _value(value < mod() ? value : value % mod()) {}

    constexpr ModIntegral(std::signed_integral auto value) {
        value %= static_cast<decltype(value)>(mod());
        _value = value < 0 ? value + mod() : value;
    }

    explicit constexpr operator bool() const {
        return _value != 0;
    }

    static constexpr T mod() {
        return modular;
    }

    constexpr T value() const {
        return _value;
    }

    constexpr ModIntegral operator-() const {
        return ModIntegral(value() == 0 ? 0 : mod() - value());
    }

    constexpr ModIntegral inverse() const {
        if constexpr (prime) {
            return power(mod() - 2);
        } else {
            T u = 0, v = 0, res = 1;
            for (T x = value(), y = mod(); y != 0; std::swap(x, y), std::swap(res, v)) {
                u = x / y, x -= u * y, res -= u * v;
            }
            return ModIntegral(res >= mod() ? res + mod() : res);
        }
    }

    constexpr ModIntegral power(std::size_t exp) const {
        ModIntegral res(1);
        for (ModIntegral base(value()); exp != 0; exp >>= 1, base *= base) {
            if (exp & 1) {
                res *= base;
            }
        }
        return res;
    }

    constexpr ModIntegral &operator/=(const ModIntegral &other) {
        *this *= other.inverse();
        return *this;
    }

    constexpr ModIntegral &operator+=(const ModIntegral &other) {
        _value += other.value();
        if (_value >= mod()) {
            _value -= mod();
        }
        return *this;
    }

    constexpr ModIntegral &operator-=(const ModIntegral &other) {
        _value -= other.value();
        if (_value >= mod()) {
            _value += mod();
        }
        return *this;
    }

    constexpr ModIntegral &operator*=(const ModIntegral &other) {
        if constexpr (std::numeric_limits<T>::digits < 64) {
            _value = static_cast<std::uint64_t>(value()) * other.value() % mod();
        } else {
            std::int64_t res = value() * other.value();
            res -= static_cast<std::uint64_t>(static_cast<long double>(value()) * other.value() / mod()) * mod();
            _value = res < 0 ? res + mod() : res >= mod() ? res - mod() : res;
        }
        return *this;
    }

    friend constexpr ModIntegral operator+(const ModIntegral &lhs, const ModIntegral &rhs) {
        return ModIntegral(lhs) += rhs;
    }

    friend constexpr ModIntegral operator-(const ModIntegral &lhs, const ModIntegral &rhs) {
        return ModIntegral(lhs) -= rhs;
    }

    friend constexpr ModIntegral operator*(const ModIntegral &lhs, const ModIntegral &rhs) {
        return ModIntegral(lhs) *= rhs;
    }

    friend constexpr ModIntegral operator/(const ModIntegral &lhs, const ModIntegral &rhs) {
        return ModIntegral(lhs) /= rhs;
    }

    friend constexpr bool operator==(const ModIntegral &lhs, const ModIntegral &rhs) {
        return lhs.value() == rhs.value();
    }

    friend constexpr std::strong_ordering operator<=>(const ModIntegral &lhs, const ModIntegral &rhs) {
        return lhs.value() <=> rhs.value();
    }

    template <typename Istream>
    friend Istream &operator>>(Istream &istream, ModIntegral &self) {
        T value;
        istream >> value;
        self = value;
        return istream;
    }

    template <typename Ostream>
    friend Ostream &operator<<(Ostream &ostream, const ModIntegral &self) {
        return ostream << self.value();
    }

private:
    T _value;
};