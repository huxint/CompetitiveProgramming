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

    constexpr auto operator~() const -> Fraction {
        return Fraction(y, x);
    }

    constexpr auto operator-() const -> Fraction {
        return Fraction(-x, y);
    }

    constexpr auto reset(T _x, T _y) -> void {
        x = _x;
        y = _y;
        reduce();
    }

    constexpr auto numerator() const -> T {
        return x;
    }

    constexpr auto denominator() const -> T {
        return y;
    }

    constexpr auto power(std::size_t exp) const -> Fraction {
        Fraction res(1);
        for (Fraction base(*this); exp != 0; exp >>= 1, base *= base) {
            if (exp & 1) {
                res *= base;
            }
        }
        return res;
    }

    constexpr auto operator+=(const Fraction &rhs) -> Fraction & {
        x = x * rhs.y + rhs.x * y;
        y *= rhs.y;
        reduce();
        return *this;
    }

    constexpr auto operator-=(const Fraction &rhs) -> Fraction & {
        x = x * rhs.y - rhs.x * y;
        y *= rhs.y;
        reduce();
        return *this;
    }

    constexpr auto operator*=(const Fraction &rhs) -> Fraction & {
        x *= rhs.x;
        y *= rhs.y;
        reduce();
        return *this;
    }

    constexpr auto operator/=(const Fraction &rhs) -> Fraction & {
        x *= rhs.y;
        y *= rhs.x;
        reduce();
        return *this;
    }

    friend constexpr auto operator+(const Fraction &lhs, const Fraction &rhs) -> Fraction {
        return Fraction(lhs) += rhs;
    }

    friend constexpr auto operator-(const Fraction &lhs, const Fraction &rhs) -> Fraction {
        return Fraction(lhs) -= rhs;
    }

    friend constexpr auto operator*(const Fraction &lhs, const Fraction &rhs) -> Fraction {
        return Fraction(lhs) *= rhs;
    }

    friend constexpr auto operator/(const Fraction &lhs, const Fraction &rhs) -> Fraction {
        return Fraction(lhs) /= rhs;
    }

    friend constexpr auto operator==(const Fraction &lhs, const Fraction &rhs) -> bool {
        return lhs.x * rhs.y == rhs.x * lhs.y;
    }

    friend constexpr auto operator<=>(const Fraction &lhs, const Fraction &rhs) -> std::strong_ordering {
        return lhs.x * rhs.y <=> rhs.x * lhs.y;
    }

    template <typename Ostream>
    friend constexpr auto operator<<(Ostream &ostream, const Fraction &self) -> Ostream & {
        if (self.y == 1) {
            return ostream << self.x;
        } else {
            return ostream << self.x << '/' << self.y;
        }
    }

private:
    T x, y;
    constexpr auto reduce() -> void {
        auto gcd = std::gcd(x, y);
        x /= gcd;
        y /= gcd;
        if (y < 0) {
            x = -x;
            y = -y;
        }
    }
};