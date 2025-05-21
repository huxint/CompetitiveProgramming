#pragma once
#include <numeric>
#include <compare>
#include <concepts>

template <std::integral T>
class Fraction {
public:
    constexpr Fraction(T _x = 0, T _y = 1) : x(_x), y(_y) {
        reduce();
    }

    explicit operator bool() const {
        return x != 0;
    }

    template <std::floating_point U>
    explicit operator U() const {
        return U(x) / y;
    }

    constexpr Fraction operator~() const {
        return Fraction(y, x);
    }

    constexpr Fraction operator-() const {
        return Fraction(-x, y);
    }

    constexpr void reset(T _x, T _y) {
        x = _x;
        y = _y;
        reduce();
    }

    constexpr T numerator() const {
        return x;
    }

    constexpr T denominator() const {
        return y;
    }

    constexpr Fraction power(std::size_t exp) const {
        Fraction res(1);
        for (Fraction base(*this); exp != 0; exp >>= 1, base *= base) {
            if (exp & 1) {
                res *= base;
            }
        }
        return res;
    }

    constexpr Fraction &operator+=(const Fraction &rhs) {
        x = x * rhs.y + rhs.x * y;
        y *= rhs.y;
        reduce();
        return *this;
    }

    constexpr Fraction &operator-=(const Fraction &rhs) {
        x = x * rhs.y - rhs.x * y;
        y *= rhs.y;
        reduce();
        return *this;
    }

    constexpr Fraction &operator*=(const Fraction &rhs) {
        x *= rhs.x;
        y *= rhs.y;
        reduce();
        return *this;
    }

    constexpr Fraction &operator/=(const Fraction &rhs) {
        x *= rhs.y;
        y *= rhs.x;
        reduce();
        return *this;
    }

    friend constexpr Fraction operator+(const Fraction &lhs, const Fraction &rhs) {
        return Fraction(lhs) += rhs;
    }

    friend constexpr Fraction operator-(const Fraction &lhs, const Fraction &rhs) {
        return Fraction(lhs) -= rhs;
    }

    friend constexpr Fraction operator*(const Fraction &lhs, const Fraction &rhs) {
        return Fraction(lhs) *= rhs;
    }

    friend constexpr Fraction operator/(const Fraction &lhs, const Fraction &rhs) {
        return Fraction(lhs) /= rhs;
    }

    friend constexpr bool operator==(const Fraction &lhs, const Fraction &rhs) {
        return lhs.x * rhs.y == rhs.x * lhs.y;
    }

    friend constexpr std::strong_ordering operator<=>(const Fraction &lhs, const Fraction &rhs) {
        return lhs.x * rhs.y <=> rhs.x * lhs.y;
    }

    template <typename Ostream>
    friend constexpr Ostream &operator<<(Ostream &ostream, const Fraction &self) {
        if (self.y == 1) {
            return ostream << self.x;
        } else {
            return ostream << self.x << '/' << self.y;
        }
    }

private:
    T x, y;
    constexpr void reduce() {
        auto gcd = std::gcd(x, y);
        x /= gcd;
        y /= gcd;
        if (y < 0) {
            x = -x;
            y = -y;
        }
    }
};