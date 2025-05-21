#pragma once
#include <vector>
#include <ranges>
#include <charconv>
#include <algorithm>

/**
 * author: huxint
 * time: 2025/4/5
 */
template <std::size_t width>
    requires(width > 0 and width < 10)
class BigInteger {
private:
    using u32 = unsigned;
    using i64 = long long;
    using u64 = unsigned long long;

protected:
    static constexpr std::size_t karatsuba_width = 6;
    static constexpr std::size_t simple_mulpty_limit = 1024;
    static constexpr std::size_t simple_inverse_limit = 16;
    static constexpr std::array<std::size_t, 10> power10 = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
    static constexpr std::size_t base = power10[width];
    static constexpr std::size_t new_base = power10[karatsuba_width];
    static constexpr std::string_view hex_chars_lower = "0123456789abcdef";
    static constexpr std::string_view hex_chars_upper = "0123456789ABCDEF";

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

    constexpr auto operator=(const char *value) -> BigInteger & {
        return *this = std::string_view(value);
    }

    constexpr auto operator=(const std::string &value) -> BigInteger & {
        return *this = std::string_view(value);
    }

    constexpr auto operator=(std::integral auto value) -> BigInteger & {
        if (value == std::numeric_limits<decltype(value)>::min()) {
            return *this = std::to_string(value);
        }
        digits.clear();
        sign = value < 0 ? -1 : value != 0;
        std::size_t x = value < 0 ? -value : value;
        do {
            digits.push_back(x % base);
            x /= base;
        } while (x != 0);
        check_zero();
        return *this;
    }

    constexpr auto operator=(std::string_view value) -> BigInteger & {
        if (value.empty()) {
            return *this = 0;
        }
        digits.clear();
        std::size_t start = not std::isdigit(value.front());
        sign = start ? value.front() == '-' ? -1 : 1 : 1;
        std::size_t size = (value.size() - start + width - 1) / width;
        digits.resize(size--);
        auto point = value.data() + start;
        auto end = value.data() + value.size();
        std::size_t remainder = (value.size() - start) % width;
        if (remainder) {
            std::from_chars(point, point + remainder, digits[size--]);
        }
        for (point += remainder; point != end; point += width) {
            std::from_chars(point, point + width, digits[size--]);
        }
        check_zero();
        return *this;
    }

    explicit operator bool() const {
        return not zero();
    }

    constexpr auto zero() const -> bool {
        return sign == 0;
    }

    constexpr auto power(std::size_t exp) const -> BigInteger {
        BigInteger res(1);
        for (BigInteger base(*this); exp != 0; exp >>= 1, base *= base) {
            if (exp & 1) {
                res *= base;
            }
        }
        return res;
    }

    constexpr auto abs() const -> BigInteger {
        BigInteger res(*this);
        res.sign = sign != 0;
        return res;
    }

    constexpr auto divby2() -> void {
        for (std::size_t i = width_size() - 1; i + 1 != 0; --i) {
            if ((digits[i] & 1) and i != 0) {
                digits[i - 1] += base;
            }
            digits[i] >>= 1;
        }
        check_zero();
    }

    constexpr auto div2() const -> BigInteger {
        BigInteger res(*this);
        res.divby2();
        return res;
    }

    constexpr auto to_signed() const -> auto {
        return std::stoll(to_string());
    }

    constexpr auto to_unsigned() const -> auto {
        return std::stoull(to_string());
    }

    constexpr auto to_binary(bool reverse = true) const -> std::string {
        std::string res;
        for (BigInteger value(*this); not value.zero(); value.divby2()) {
            res += (value.digits.front() & 1) | 0X30;
        }
        if (reverse) {
            std::reverse(res.begin(), res.end());
        }
        return res;
    }

    constexpr auto to_hex(bool upper = false) const -> std::string {
        auto &hex_chars(upper ? hex_chars_upper : hex_chars_lower);
        std::string res;
        std::string str = to_binary();
        res.reserve(width_size() / 4);
        for (auto value : str | std::ranges::views::reverse | std::ranges::views::chunk(4)) {
            res += hex_chars[(value[3] & 1) * 8 + (value[2] & 1) * 4 + (value[1] & 1) * 2 + (value[0] & 1)];
        }
        return res;
    }

    constexpr auto to_string() const -> std::string {
        std::string res;
        res.reserve(width_size() * width + 1);
        if (sign == -1) {
            res += '-';
        }
        res += std::to_string(digits.back());
        std::string str;
        for (auto value : digits | std::views::reverse | std::views::drop(1)) {
            str = std::to_string(value);
            res += str.insert(0, width - str.size(), '0');
        }
        return res;
    }

    constexpr auto operator~() const -> BigInteger {
        return -(*this) - 1;
    }

    constexpr auto operator+() const -> BigInteger {
        return *this;
    }

    constexpr auto operator-() const -> BigInteger {
        BigInteger res(*this);
        res.sign = -sign;
        return res;
    }

    constexpr auto operator++(int) -> BigInteger {
        BigInteger res(*this);
        ++*this;
        return res;
    }

    constexpr auto operator--(int) -> BigInteger {
        BigInteger res(*this);
        --*this;
        return res;
    }

    constexpr auto operator++() -> BigInteger & {
        return *this += 1;
    }

    constexpr auto operator--() -> BigInteger & {
        return *this -= 1;
    }

    constexpr auto operator<<(i64 shift) -> BigInteger {
        return BigInteger(*this) <<= shift;
    }

    constexpr auto operator>>(i64 shift) -> BigInteger {
        return BigInteger(*this) >>= shift;
    }

    constexpr auto operator<<=(i64 shift) -> BigInteger & {
        if (shift <= 0) {
            return *this;
        }
        for (i64 i = 0; i != shift; ++i) {
            *this += *this;
        }
        return *this;
    }

    constexpr auto operator>>=(i64 shift) -> BigInteger & {
        if (shift <= 0) {
            return *this;
        }
        for (i64 i = 0; i != shift; ++i) {
            *this->divby2();
        }
        return *this;
    }

    constexpr auto operator&=(const BigInteger &other) -> BigInteger & {
        return binary_op_helper(other, [](int lhs, int rhs) -> int {
            return lhs & rhs;
        });
    }

    constexpr auto operator|=(const BigInteger &other) -> BigInteger & {
        return binary_op_helper(other, [](int lhs, int rhs) -> int {
            return lhs | rhs;
        });
    }

    constexpr auto operator^=(const BigInteger &other) -> BigInteger & {
        return binary_op_helper(other, [](int lhs, int rhs) -> int {
            return lhs ^ rhs;
        });
    }

    constexpr auto operator+=(const BigInteger &other) -> BigInteger & {
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

    constexpr auto operator-=(const BigInteger &other) -> BigInteger & {
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

    constexpr auto operator*=(const BigInteger &other) -> BigInteger & {
        if (zero()) {
            return *this;
        }
        if (other.zero()) {
            return *this = other;
        }
        sign = sign * other.sign;
        digits = width_size() * other.width_size() > simple_mulpty_limit ? karatsuba_multiply(digits, other.digits) : simple_multiply(digits, other.digits);
        normalize();
        return *this;
    }

    constexpr auto operator/=(const BigInteger &other) -> BigInteger & {
        // Todo
    }

    constexpr auto operator%=(const BigInteger &other) -> BigInteger & {
        // Todo
    }

    friend constexpr auto operator&(const BigInteger &lhs, const BigInteger &rhs) -> BigInteger {
        return BigInteger(lhs) &= rhs;
    }

    friend constexpr auto operator|(const BigInteger &lhs, const BigInteger &rhs) -> BigInteger {
        return BigInteger(lhs) |= rhs;
    }

    friend constexpr auto operator^(const BigInteger &lhs, const BigInteger &rhs) -> BigInteger {
        return BigInteger(lhs) ^= rhs;
    }

    friend constexpr auto operator+(const BigInteger &lhs, const BigInteger &rhs) -> BigInteger {
        return BigInteger(lhs) += rhs;
    }

    friend constexpr auto operator-(const BigInteger &lhs, const BigInteger &rhs) -> BigInteger {
        return BigInteger(lhs) -= rhs;
    }

    friend constexpr auto operator*(const BigInteger &lhs, const BigInteger &rhs) -> BigInteger {
        return BigInteger(lhs) *= rhs;
    }

    friend constexpr auto operator/(const BigInteger &lhs, const BigInteger &rhs) -> BigInteger {
        return BigInteger(lhs) /= rhs;
    }

    friend constexpr auto operator%(const BigInteger &lhs, const BigInteger &rhs) -> BigInteger {
        return BigInteger(lhs) %= rhs;
    }

    friend constexpr auto operator==(const BigInteger &lhs, const BigInteger &rhs) -> bool {
        if (lhs.sign != rhs.sign) {
            return false;
        }
        return lhs.digits == rhs.digits;
    }

    friend constexpr auto operator<=>(const BigInteger &lhs, const BigInteger &rhs) -> std::strong_ordering {
        if (lhs.sign != rhs.sign) {
            return lhs.sign <=> rhs.sign;
        } else if (lhs.zero()) {
            return std::strong_ordering::equal;
        } else if (lhs.sign == 1) {
            return lhs.compare_abs(rhs);
        } else {
            return rhs.compare_abs(lhs);
        }
    }

    template <typename Istream>
    friend constexpr auto operator>>(Istream &istream, BigInteger &self) -> Istream & {
        std::string value;
        istream >> value;
        self = value;
        return istream;
    }

    template <typename Ostream>
    friend constexpr auto operator<<(Ostream &ostream, const BigInteger &self) -> Ostream & {
        if (self.sign == -1) {
            ostream << '-';
        }
        ostream << self.digits.back();
        for (auto value : self.digits | std::ranges::views::reverse | std::ranges::views::drop(1)) {
            for (std::size_t i = 0; i != width; ++i) {
                if (value < power10[width - i - 1]) {
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
    std::vector<u32> digits;

    constexpr auto width_size() const -> std::size_t {
        return digits.size();
    }

    constexpr auto compare_abs(const BigInteger &other) const -> std::strong_ordering {
        return width_size() != other.width_size() ? width_size() <=> other.width_size() : std::lexicographical_compare_three_way(digits.rbegin(), digits.rend(), other.digits.rbegin(), other.digits.rend());
    }

    constexpr auto check_zero() -> void {
        if (normalize(); digits.size() == 1 and digits.back() == 0) {
            sign = 0;
        }
    }

    constexpr auto normalize() -> void {
        while (digits.size() > 1 and digits.back() == 0) {
            digits.pop_back();
        }
    }

    constexpr auto binary_op_helper(const BigInteger &other, auto &&op) -> BigInteger & {
        std::string lhs = to_binary(false);
        std::string rhs = other.to_binary(false);
        *this = 0;
        for (std::size_t i = std::max(lhs.size(), rhs.size()) - 1; i + 1 != 0; --i) {
            *this += *this;
            *this += op(i < lhs.size() ? (lhs[i] & 1) : 0, i < rhs.size() ? (rhs[i] & 1) : 0);
        }
        return *this;
    }

    constexpr auto plus_impl(const BigInteger &other) -> void {
        bool carry = false;
        std::size_t i = 0;
        std::size_t size = other.width_size();
        if (width_size() < size) {
            digits.resize(size);
        }
        for (; i < size; ++i) {
            digits[i] += other.digits[i] + carry;
            if (carry = digits[i] >= base; carry) {
                digits[i] -= base;
            }
        }
        for (; carry; ++i) {
            if (i == width_size()) {
                digits.push_back(0);
            }
            digits[i] += carry;
            if (carry = digits[i] >= base; carry) {
                digits[i] -= base;
            }
        }
        check_zero();
    }

    constexpr auto minus_impl(const BigInteger &other) -> void {
        bool carry = false;
        auto current = digits.data();
        std::size_t size = other.width_size();
        for (std::size_t i = 0; i < size or carry; ++i, ++current) {
            std::size_t value = carry + (i < size ? other.digits[i] : 0);
            if (carry = value > *current; carry) {
                *current -= value - base;
            } else {
                *current -= value;
            }
        }
        check_zero();
    }

    constexpr auto simple_multiply(const std::vector<u32> &lhs, const std::vector<u32> &rhs) -> std::vector<u32> {
        std::vector<u32> res(lhs.size() + rhs.size());
        for (std::size_t i = 0; i < lhs.size(); ++i) {
            for (std::size_t j = 0, carry = 0; (lhs[i] != 0 and j < rhs.size()) or carry != 0; ++j) {
                res[i + j] = (carry += res[i + j] + std::size_t(lhs[i]) * (j < rhs.size() ? rhs[j] : 0)) % base;
                carry /= base;
            }
        }
        while (res.size() > 1 and res.back() == 0) {
            res.pop_back();
        }
        return res;
    }

    static constexpr auto convert_base(const std::vector<u32> &vector, std::size_t old_width, std::size_t new_width) -> std::vector<u32> {
        std::vector<u32> res;
        std::size_t carry = 0;
        for (std::size_t i = 0, _width = 0; i < vector.size(); ++i) {
            carry += vector[i] * power10[_width];
            for (_width += old_width; _width >= new_width; _width -= new_width) {
                res.push_back(carry % power10[new_width]);
                carry /= power10[new_width];
            }
        }
        res.push_back(carry);
        while (res.size() > 1 and res.back() == 0) {
            res.pop_back();
        }
        return res;
    }

    constexpr auto karatsuba_multiply(const std::vector<u32> &lhs, const std::vector<u32> &rhs) -> std::vector<u32> {
        std::vector<u32> new_lhs = convert_base(lhs, width, karatsuba_width);
        std::vector<u32> new_rhs = convert_base(rhs, width, karatsuba_width);
        std::vector<u64> x(new_lhs.begin(), new_lhs.end());
        std::vector<u64> y(new_rhs.begin(), new_rhs.end());
        std::size_t max_bit_ceil = std::bit_ceil(std::max(x.size(), y.size()));
        x.resize(max_bit_ceil);
        y.resize(max_bit_ceil);
        auto karatsuba = [](auto &&self, const std::vector<u64> &lhs, const std::vector<u64> &rhs) -> std::vector<u64> {
            std::size_t size = lhs.size();
            std::vector<u64> res(2 * size);
            if (size <= 32) {
                for (std::size_t i = 0; i < size; ++i) {
                    for (std::size_t j = 0; lhs[i] != 0 and j < size; ++j) {
                        res[i + j] += lhs[i] * rhs[j];
                    }
                }
                return res;
            }
            std::size_t split = size / 2;
            std::vector<u64> lhs_low(lhs.begin(), lhs.begin() + split);
            std::vector<u64> lhs_high(lhs.begin() + split, lhs.end());
            std::vector<u64> rhs_low(rhs.begin(), rhs.begin() + split);
            std::vector<u64> rhs_high(rhs.begin() + split, rhs.end());
            std::vector<u64> low = self(self, lhs_low, rhs_low);
            std::vector<u64> high = self(self, lhs_high, rhs_high);
            for (std::size_t i = 0; i < split; ++i) {
                lhs_high[i] += lhs_low[i];
                rhs_high[i] += rhs_low[i];
            }
            std::vector<u64> mid = self(self, lhs_high, rhs_high);
            for (std::size_t i = 0; i < low.size(); ++i) {
                mid[i] -= low[i];
                res[i] += low[i];
            }
            for (std::size_t i = 0; i < high.size(); ++i) {
                mid[i] -= high[i];
                res[i + size] += high[i];
            }
            for (std::size_t i = 0; i < mid.size(); ++i) {
                res[i + split] += mid[i];
            }
            return res;
        };
        std::vector<u64> multiply = karatsuba(karatsuba, x, y);
        std::vector<u32> res(multiply.size());
        for (std::size_t i = 0, carry = 0; i < multiply.size(); ++i) {
            res[i] = (carry += multiply[i]) % new_base;
            carry /= new_base;
        }
        return convert_base(res, karatsuba_width, width);
    }

    constexpr auto divide_impl(const BigInteger &other) -> void {}

};