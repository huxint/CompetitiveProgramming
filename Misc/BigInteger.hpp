#pragma once
#include <vector>
#include <ranges>
#include <charconv>
#include <algorithm>
#include <cstdint>

/**
 * author: huxint
 * time: 2025/4/5
 */
template <std::size_t width>
    requires(width > 0 and width < 10)
class BigInteger {
protected:
    static constexpr std::size_t karatsuba_width = 6;
    static constexpr std::size_t simple_mulpty_limit = 1024;
    static constexpr std::size_t simple_inverse_limit = 16;
    static constexpr std::array<std::uint32_t, 10> pow10 = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
    static constexpr std::uint32_t base = pow10[width];
    static constexpr std::uint32_t karatsuba_base = pow10[karatsuba_width];

public:
    constexpr BigInteger() {
        *this = 0;
    }

    constexpr BigInteger(std::integral auto value) {
        *this = value;
    }

    constexpr BigInteger(std::string_view value) {
        *this = value;
    }

    constexpr BigInteger(const char *value) {
        *this = std::string_view(value);
    }

    constexpr BigInteger(const std::string &value) {
        *this = std::string_view(value);
    }

    constexpr BigInteger &operator=(const char *value) {
        return *this = std::string_view(value);
    }

    constexpr BigInteger &operator=(const std::string &value) {
        return *this = std::string_view(value);
    }

    constexpr BigInteger &operator=(std::integral auto value) {
        if (value == std::numeric_limits<decltype(value)>::min()) {
            return *this = std::to_string(value);
        }
        digits.clear();
        sign = value < 0 ? -1 : value != 0;
        auto x = value < 0 ? -value : value;
        do {
            digits.push_back(x % base);
            x /= base;
        } while (x != 0);
        normalize();
        return *this;
    }

    constexpr BigInteger &operator=(std::string_view value) {
        if (value.empty()) {
            return *this = 0;
        }
        digits.clear();
        std::size_t start = not std::isdigit(value.front());
        sign = start ? value.front() == '-' ? -1 : 1 : 1;
        std::size_t _size = (value.size() - start + width - 1) / width;
        digits.resize(_size--);
        auto point = value.data() + start;
        auto end = value.data() + value.size();
        std::size_t rem = (value.size() - start) % width;
        if (rem) {
            std::from_chars(point, point + rem, at(--_size));
        }
        for (point += rem; point != end; point += width) {
            std::from_chars(point, point + width, at(--_size));
        }
        normalize();
        return *this;
    }

    explicit operator bool() const {
        return not zero();
    }

    constexpr bool zero() const {
        return sign == 0;
    }

    constexpr BigInteger power(std::size_t exp) const {
        BigInteger res(1);
        for (BigInteger base(*this); exp != 0; exp >>= 1, base *= base) {
            if (exp & 1) {
                res *= base;
            }
        }
        return res;
    }

    constexpr BigInteger abs() const {
        BigInteger res(*this);
        res.sign = sign != 0;
        return res;
    }

    constexpr long long to_signed() const {
        return std::stoll(to_string());
    }

    constexpr unsigned long long to_unsigned() const {
        return std::stoull(to_string());
    }

    constexpr std::string to_binary(bool reverse = true) const {
        std::string res;
        for (BigInteger value(*this); value; value /= 2) {
            res += (value.at(0) & 1) | 0X30;
        }
        if (reverse) {
            std::reverse(res.begin(), res.end());
        }
        return res;
    }

    constexpr std::string to_string() const {
        std::string res;
        res.reserve(size() * width + 1);
        if (sign == -1) {
            res += '-';
        }
        res += std::to_string(at(size() - 1));
        std::string str;
        for (auto value : digits | std::views::reverse | std::views::drop(1)) {
            str = std::to_string(value);
            res += str.insert(0, width - str.size(), '0');
        }
        return res;
    }

    constexpr BigInteger operator~() const {
        return -(*this) - 1;
    }

    constexpr BigInteger operator+() const {
        return *this;
    }

    constexpr BigInteger operator-() const {
        BigInteger res(*this);
        res.sign = -sign;
        return res;
    }

    constexpr BigInteger operator++(int) {
        BigInteger res(*this);
        ++*this;
        return res;
    }

    constexpr BigInteger operator--(int) {
        BigInteger res(*this);
        --*this;
        return res;
    }

    constexpr BigInteger &operator++() {
        return *this += 1;
    }

    constexpr BigInteger &operator--() {
        return *this -= 1;
    }

    constexpr BigInteger operator<<(std::int64_t shift) {
        return BigInteger(*this) <<= shift;
    }

    constexpr BigInteger operator>>(std::int64_t shift) {
        return BigInteger(*this) >>= shift;
    }

    constexpr BigInteger &operator<<=(std::int64_t shift) {
        if (shift <= 0) {
            return *this;
        }
        for (std::int64_t i = 0; i != shift; ++i) {
            *this += *this;
        }
        return *this;
    }

    constexpr BigInteger &operator>>=(std::int64_t shift) {
        if (shift <= 0) {
            return *this;
        }
        for (std::int64_t i = 0; i != shift; ++i) {
            *this /= 2;
        }
        return *this;
    }

    constexpr BigInteger &operator&=(const BigInteger &other) {
        return binary_op_helper(other, std::bit_and<>());
    }

    constexpr BigInteger &operator|=(const BigInteger &other) {
        return binary_op_helper(other, std::bit_or<>());
    }

    constexpr BigInteger &operator^=(const BigInteger &other) {
        return binary_op_helper(other, std::bit_xor<>());
    }

    constexpr BigInteger &operator+=(const BigInteger &other) {
        if (sign == other.sign) {
            plus_impl(other);
        } else if (compare_abs(other) >= 0) {
            minus_impl(other);
        } else {
            auto _other = other;
            std::swap(*this, _other);
            minus_impl(_other);
        }
        return *this;
    }

    constexpr BigInteger &operator-=(const BigInteger &other) {
        if (sign == other.sign) {
            if (compare_abs(other) >= 0) {
                minus_impl(other);
            } else {
                auto _other = other;
                std::swap(*this, _other);
                minus_impl(_other);
                sign = -sign;
            }
        } else {
            plus_impl(other);
        }
        return *this;
    }

    constexpr BigInteger &operator*=(const BigInteger &other) {
        if (zero()) {
            return *this;
        }
        if (other.zero()) {
            return *this = other;
        }
        sign = sign * other.sign;
        digits = size() * other.size() > simple_mulpty_limit ? karatsuba_multiply(digits, other.digits) : simple_multiply(digits, other.digits);
        normalize();
        return *this;
    }

    constexpr BigInteger &operator/=(std::int64_t other) {
        if (other == 2) {
            for (std::size_t i = size(); i-- != 0;) {
                if ((at(i) & 1) and i != 0) {
                    at(i - 1) += base;
                }
                at(i) >>= 1;
            }
            normalize();
        } else {
            // Todo
        }
        return *this;
    }

    constexpr BigInteger &operator%=(std::int64_t other) {
        if (other == 2) {
            digits.erase(digits.begin() + 1, digits.end());
            at(0) &= 1;
        } else {
            // Todo
        }
        return *this;
    }

    constexpr BigInteger &operator/=(const BigInteger &other) {
        // Todo
    }

    constexpr BigInteger &operator%=(const BigInteger &other) {
        // Todo
    }

    friend constexpr BigInteger operator&(const BigInteger &lhs, const BigInteger &rhs) {
        return BigInteger(lhs) &= rhs;
    }

    friend constexpr BigInteger operator|(const BigInteger &lhs, const BigInteger &rhs) {
        return BigInteger(lhs) |= rhs;
    }

    friend constexpr BigInteger operator^(const BigInteger &lhs, const BigInteger &rhs) {
        return BigInteger(lhs) ^= rhs;
    }

    friend constexpr BigInteger operator+(const BigInteger &lhs, const BigInteger &rhs) {
        return BigInteger(lhs) += rhs;
    }

    friend constexpr BigInteger operator-(const BigInteger &lhs, const BigInteger &rhs) {
        return BigInteger(lhs) -= rhs;
    }

    friend constexpr BigInteger operator*(const BigInteger &lhs, const BigInteger &rhs) {
        return BigInteger(lhs) *= rhs;
    }

    friend constexpr BigInteger operator/(const BigInteger &lhs, const BigInteger &rhs) {
        return BigInteger(lhs) /= rhs;
    }

    friend constexpr BigInteger operator%(const BigInteger &lhs, const BigInteger &rhs) {
        return BigInteger(lhs) %= rhs;
    }

    friend constexpr bool operator==(const BigInteger &lhs, const BigInteger &rhs) {
        if (lhs.sign != rhs.sign) {
            return false;
        }
        return lhs.digits == rhs.digits;
    }

    friend constexpr std::strong_ordering operator<=>(const BigInteger &lhs, const BigInteger &rhs) {
        if (lhs.sign != rhs.sign) {
            return lhs.sign <=> rhs.sign;
        } else if (lhs) {
            return std::strong_ordering::equal;
        } else {
            return lhs.sign == 1 ? lhs.compare_abs(rhs) : rhs.compare_abs(lhs);
        }
    }

    template <typename Istream>
    friend Istream &operator>>(Istream &istream, BigInteger &self) {
        std::string value;
        istream >> value;
        self = value;
        return istream;
    }

    template <typename Ostream>
    friend Ostream &operator<<(Ostream &ostream, const BigInteger &self) {
        if (self.sign == -1) {
            ostream << '-';
        }
        ostream << self.digits.back();
        for (auto value : self.digits | std::ranges::views::reverse | std::ranges::views::drop(1)) {
            for (std::size_t i = 0; i != width; ++i) {
                if (value < pow10[width - i - 1]) {
                    ostream << '0';
                } else {
                    ostream << value;
                    break;
                }
            }
        }
        return ostream;
    }

private:
    int sign;
    std::vector<std::uint32_t> digits;

    constexpr std::uint32_t &at(std::size_t index) {
        return digits.at(index);
    }

    constexpr std::uint32_t at(std::size_t index) const {
        return digits.at(index);
    }

    constexpr std::size_t size() const {
        return digits.size();
    }

    constexpr std::strong_ordering compare_abs(const BigInteger &other) const {
        return size() != other.size() ? size() <=> other.size() : std::lexicographical_compare_three_way(digits.rbegin(), digits.rend(), other.digits.rbegin(), other.digits.rend());
    }

    constexpr void normalize() {
        while (digits.size() > 1 and digits.back() == 0) {
            digits.pop_back();
        }
        if (digits.size() == 1 and digits.back() == 0) {
            sign = 0;
        }
    }

    constexpr BigInteger &binary_op_helper(const BigInteger &other, auto &&op) {
        std::string lhs = to_binary(false);
        std::string rhs = other.to_binary(false);
        *this = 0;
        for (std::size_t i = std::max(lhs.size(), rhs.size()); i-- != 0;) {
            *this += *this;
            *this += op(i < lhs.size() ? (lhs[i] & 1) : 0, i < rhs.size() ? (rhs[i] & 1) : 0);
        }
        return *this;
    }

    constexpr void plus_impl(const BigInteger &other) {
        bool carry = false;
        std::size_t i = 0;
        std::size_t _size = other.size();
        if (size() < _size) {
            digits.resize(_size);
        }
        for (; i < _size; ++i) {
            at(i) += other.at(i) + carry;
            if (carry = at(i) >= base; carry) {
                at(i) -= base;
            }
        }
        for (; carry; ++i) {
            if (i == size()) {
                digits.push_back(0);
            }
            at(i) += carry;
            if (carry = at(i) >= base; carry) {
                at(i) -= base;
            }
        }
        normalize();
    }

    constexpr void minus_impl(const BigInteger &other) {
        bool carry = false;
        auto it = digits.data();
        std::size_t _size = other.size();
        for (std::size_t i = 0; i < _size or carry; ++i, ++it) {
            std::size_t value = carry + (i < _size ? other.at(i) : 0);
            if (carry = value > *it; carry) {
                *it -= value - base;
            } else {
                *it -= value;
            }
        }
        normalize();
    }

    constexpr std::vector<std::uint32_t> simple_multiply(const std::vector<std::uint32_t> &lhs, const std::vector<std::uint32_t> &rhs) {
        std::vector<std::uint32_t> res(lhs.size() + rhs.size());
        for (std::size_t i = 0; i < lhs.size(); ++i) {
            std::uint64_t carry = 0;
            for (std::size_t j = 0; (lhs[i] != 0 and j < rhs.size()) or carry != 0; ++j) {
                res[i + j] = (carry += res[i + j] + std::uint64_t(lhs[i]) * (j < rhs.size() ? rhs[j] : 0)) % base;
                carry /= base;
            }
        }
        while (res.size() > 1 and res.back() == 0) {
            res.pop_back();
        }
        return res;
    }

    static constexpr std::vector<std::uint32_t> convert_base(const std::vector<std::uint32_t> &vector, std::size_t old_width, std::size_t new_width) {
        std::vector<std::uint32_t> res;
        std::uint64_t carry = 0;
        for (std::size_t i = 0, _width = 0; i < vector.size(); ++i) {
            carry += vector[i] * pow10[_width];
            for (_width += old_width; _width >= new_width; _width -= new_width) {
                res.push_back(carry % pow10[new_width]);
                carry /= pow10[new_width];
            }
        }
        res.push_back(carry);
        while (res.size() > 1 and res.back() == 0) {
            res.pop_back();
        }
        return res;
    }

    constexpr std::vector<std::uint32_t> karatsuba_multiply(const std::vector<std::uint32_t> &lhs, const std::vector<std::uint32_t> &rhs) {
        std::vector<std::uint32_t> _lhs = convert_base(lhs, width, karatsuba_width);
        std::vector<std::uint32_t> _rhs = convert_base(rhs, width, karatsuba_width);
        std::vector<std::uint64_t> x(_lhs.begin(), _lhs.end());
        std::vector<std::uint64_t> y(_rhs.begin(), _rhs.end());
        std::size_t max = std::bit_ceil(std::max(x.size(), y.size()));
        x.resize(max);
        y.resize(max);
        auto karatsuba = [](auto &&self, const std::vector<std::uint64_t> &lhs, const std::vector<std::uint64_t> &rhs) -> std::vector<std::uint64_t> {
            std::size_t _size = lhs.size();
            std::vector<std::uint64_t> res(2 * _size);
            if (_size <= 32) {
                for (std::size_t i = 0; i < _size; ++i) {
                    for (std::size_t j = 0; lhs[i] != 0 and j < _size; ++j) {
                        res[i + j] += lhs[i] * rhs[j];
                    }
                }
                return res;
            }
            std::size_t split = _size / 2;
            std::vector<std::uint64_t> lhs_low(lhs.begin(), lhs.begin() + split);
            std::vector<std::uint64_t> lhs_high(lhs.begin() + split, lhs.end());
            std::vector<std::uint64_t> rhs_low(rhs.begin(), rhs.begin() + split);
            std::vector<std::uint64_t> rhs_high(rhs.begin() + split, rhs.end());
            std::vector<std::uint64_t> low = self(self, lhs_low, rhs_low);
            std::vector<std::uint64_t> high = self(self, lhs_high, rhs_high);
            for (std::size_t i = 0; i < split; ++i) {
                lhs_high[i] += lhs_low[i];
                rhs_high[i] += rhs_low[i];
            }
            std::vector<std::uint64_t> mid = self(self, lhs_high, rhs_high);
            for (std::size_t i = 0; i < low.size(); ++i) {
                mid[i] -= low[i];
                res[i] += low[i];
            }
            for (std::size_t i = 0; i < high.size(); ++i) {
                mid[i] -= high[i];
                res[i + _size] += high[i];
            }
            for (std::size_t i = 0; i < mid.size(); ++i) {
                res[i + split] += mid[i];
            }
            return res;
        };
        std::vector<std::uint64_t> multiply = karatsuba(karatsuba, x, y);
        std::vector<std::uint32_t> res(multiply.size());
        for (std::size_t i = 0, carry = 0; i < multiply.size(); ++i) {
            res[i] = (carry += multiply[i]) % karatsuba_base;
            carry /= karatsuba_base;
        }
        return convert_base(res, karatsuba_width, width);
    }

    constexpr void divide_impl(const BigInteger &other) {
        // Todo
    }
};