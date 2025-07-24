/**
 * @brief 动态取模整数类
 * @details 支持运行时设置模数的取模整数类，配合Barrett算法使用
 *          支持四则运算、幂运算、逆元、比较运算、输入输出等，可隐式转换为bool
 *          使用示例：using modint = ModIntegral<uint32_t, Barrett32, 0>; modint::setmod(mod);
 *          id参数用于区分不同的动态模数实例
 * @complexity 基本运算: O(1), 幂运算: O(log n)
 */
#pragma once
#include <cstdint>
#include <concepts>
template <std::unsigned_integral type, typename Base, std::size_t id>
    requires requires (Base object) {
    object.mod();
    object.setmod(0);
    object.multiply(0, 0);
}
class ModIntegral {
public:
    ModIntegral() : _value() {}

    ModIntegral(std::unsigned_integral auto value) : _value(object.multiply(value, 1)) {}

    ModIntegral(std::signed_integral auto value) {
        value %= static_cast<decltype(value)>(mod());
        _value = value < 0 ? value + mod() : value;
    }

    explicit operator bool() const {
        return _value != 0;
    }

    static type mod() {
        return object.mod();
    }

    static void setmod(type modular, bool p = false) {
        object.setmod(modular);
        prime = p;
    }

    type value() const {
        return _value;
    }

    ModIntegral operator-() const {
        return ModIntegral(value() == 0 ? 0 : mod() - value());
    }

    ModIntegral inverse() const {
        if (prime) {
            return power(mod() - 2);
        } else {
            type u = 0, v = 0, res = 1;
            for (type x = value(), y = mod(); y != 0; std::swap(x, y), std::swap(res, v)) {
                u = x / y, x -= u * y, res -= u * v;
            }
            return ModIntegral(res >= mod() ? res + mod() : res);
        }
    }

    ModIntegral power(std::size_t exp) const {
        ModIntegral res(1);
        for (ModIntegral base(value()); exp != 0; exp >>= 1, base *= base) {
            if (exp & 1) {
                res *= base;
            }
        }
        return res;
    }

    ModIntegral &operator*=(const ModIntegral &other) {
        _value = object.multiply(value(), other.value());
        return *this;
    }

    ModIntegral &operator/=(const ModIntegral &other) {
        return *this *= other.inverse();
    }

    ModIntegral &operator+=(const ModIntegral &other) {
        _value += other.value();
        if (_value >= mod()) {
            _value -= mod();
        }
        return *this;
    }

    ModIntegral &operator-=(const ModIntegral &other) {
        _value -= other.value();
        if (_value >= mod()) {
            _value += mod();
        }
        return *this;
    }

    friend ModIntegral operator+(const ModIntegral &lhs, const ModIntegral &rhs) {
        return ModIntegral(lhs) += rhs;
    }

    friend ModIntegral operator-(const ModIntegral &lhs, const ModIntegral &rhs) {
        return ModIntegral(lhs) -= rhs;
    }

    friend ModIntegral operator*(const ModIntegral &lhs, const ModIntegral &rhs) {
        return ModIntegral(lhs) *= rhs;
    }

    friend ModIntegral operator/(const ModIntegral &lhs, const ModIntegral &rhs) {
        return ModIntegral(lhs) /= rhs;
    }

    friend bool operator==(const ModIntegral &lhs, const ModIntegral &rhs) {
        return lhs.value() == rhs.value();
    }

    friend std::strong_ordering operator<=>(const ModIntegral &lhs, const ModIntegral &rhs) {
        return lhs.value() <=> rhs.value();
    }

    template <typename Istream>
    friend Istream &operator>>(Istream &istream, ModIntegral &self) {
        type value;
        istream >> value;
        self = value;
        return istream;
    }

    template <typename Ostream>
    friend Ostream &operator<<(Ostream &ostream, const ModIntegral &self) {
        return ostream << self.value();
    }

private:
    type _value;
    static bool prime;
    static Base object;
};
template <std::unsigned_integral type, typename Base, std::size_t id>
    requires requires (Base object) {
    object.mod();
    object.setmod(0);
    object.multiply(0, 0);
}
bool ModIntegral<type, Base, id>::prime = false;
template <std::unsigned_integral type, typename Base, std::size_t id>
    requires requires (Base object) {
    object.mod();
    object.setmod(0);
    object.multiply(0, 0);
}
Base ModIntegral<type, Base, id>::object = 1000000007U;