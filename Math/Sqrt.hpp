/**
 * @brief 扩展根号类
 * @details 表示形如 a + b√V 的数，支持四则运算、幂运算、比较运算和输出
 *          使用示例：Sqrt<5, int> a(1, 2); 表示 1 + 2√5
 *          可用于计算斐波那契数列等涉及根号的数学问题
 * @complexity 基本运算: O(1), 幂运算: O(log n)
 */
#pragma once
#include <cstdint>
template <std::size_t V, typename T>
class Sqrt {
public:
    constexpr Sqrt() {}

    constexpr Sqrt(T u = 0, T v = 0) : _u(u), _v(v) {}

    explicit constexpr operator bool() const {
        return u() != 0 and v() != 0;
    }

    constexpr Sqrt operator -() const {
        return Sqrt(-u(), -v());
    }

    constexpr T u() const {
        return _u;
    }

    constexpr T v() const {
        return _v;
    }

    constexpr T &u() {
        return _u;
    }

    constexpr T &v() {
        return _v;
    }

    constexpr Sqrt &operator +=(const Sqrt &other) {
        u() += other.u();
        v() += other.v();
        return *this;
    }

    constexpr Sqrt &operator -=(const Sqrt &other) {
        u() -= other.u();
        v() -= other.v();
        return *this;
    }

    constexpr Sqrt &operator *=(const Sqrt &other) {
        return *this = Sqrt(u() * other.u() + V * v() * other.v(), u() * other.v() + v() * other.u());
    }

    constexpr Sqrt &operator /=(const Sqrt &other) {
        return *this = Sqrt((u() * other.u() - V * v() * other.v()) / (other.u() * other.u() - V * other.v() * other.v()), (v() * other.u() - u() * other.v()) / (other.u() * other.u() - V * other.v() * other.v()));
    }

    friend constexpr Sqrt operator +(const Sqrt &lhs, const Sqrt &rhs) {
        return Sqrt(lhs) += rhs;
    }

    friend constexpr Sqrt operator -(const Sqrt &lhs, const Sqrt &rhs) {
        return Sqrt(lhs) -= rhs;
    }

    friend constexpr Sqrt operator *(const Sqrt &lhs, const Sqrt &rhs) {
        return Sqrt(lhs) *= rhs;
    }

    friend constexpr Sqrt operator /(const Sqrt &lhs, const Sqrt &rhs) {
        return Sqrt(lhs) /= rhs;
    }

    friend constexpr bool operator ==(const Sqrt &lhs, const Sqrt &rhs) {
        return lhs.u() == rhs.u() and lhs.v() == rhs.v();
    }

    friend constexpr bool operator !=(const Sqrt &lhs, const Sqrt &rhs) {
        return not (lhs == rhs);
    }

    constexpr Sqrt power(std::size_t exp) const {
        Sqrt res(1);
        for (Sqrt base(*this); exp != 0; exp >>= 1, base *= base) {
            if (exp & 1) {
                res *= base;
            }
        }
        return res;
    }

    template <typename Ostream>
    friend Ostream &operator <<(Ostream &ostream, const Sqrt &self) {
        return ostream << self.u() << " + " << self.v() << " sqrt(" << V << ")";
    }

private:
    T _u, _v;// u + v sqrt(V)
};