/**
 * @brief 编译期取模整数类
 * @details 支持编译期已知模数的取模整数运算，模数必须在编译期确定
 *          支持32位和64位模数，布尔参数表示模数是否为质数（影响逆元计算方法）
 *          支持四则运算、幂运算、逆元、比较运算、输入输出等，可隐式转换为bool
 *          使用示例：using modint = ModIntegral<998244353U, true>（32位）
 *                   using modint = ModIntegral<998244353ULL, false>（64位）
 * @complexity 基本运算: O(1), 幂运算: O(log n)
 */
#pragma once
#include <cstdint>
#include <concepts>
#include <type_traits>
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