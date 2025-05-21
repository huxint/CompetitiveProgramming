#pragma once

#include <array>
#include <limits>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <string_view>

template <std::size_t width, std::array<std::uint64_t, width> base, std::array<std::uint64_t, width> modular>
class StringHash {
private:
    using hash = std::array<std::uint64_t, width>;

public:
    StringHash() {}

    StringHash(std::string_view str) {
        init(str.begin(), str.end());
    }

    template <typename Iterator>
    StringHash(Iterator begin, Iterator end) {
        init(begin, end);
    }

    void init(std::string_view str) {
        init(str.begin(), str.end());
    }

    template <typename Iterator>
    void init(Iterator begin, Iterator end) {
        _size = end - begin;
        power.resize(size() + 1);
        table.resize(size() + 1);
        std::fill(power[0].begin(), power[0].end(), 1);
        std::fill(table[0].begin(), table[0].end(), 0);
        for (std::size_t i = 0; i < size(); ++i) {
            for (std::size_t j = 0; j < width; ++j) {
                power[i + 1][j] = power[i][j] * base[j] % modular[j];
                table[i + 1][j] = table[i][j] * base[j] % modular[j] + *begin++;
                if (table[i + 1][j] >= modular[j]) {
                    table[i + 1][j] -= modular[j];
                }
            }
        }
    }

    std::size_t size() const {
        return _size;
    }

    hash get() const {
        return table[size()];
    }

    hash get(std::size_t l, std::size_t r) const {
        hash res{};
        for (std::size_t i = 0; i < width; ++i) {
            res[i] = table[r + 1][i] - table[l][i] * power[r - l + 1][i] % modular[i];
            if (res[i] >= modular[i]) {
                res[i] += modular[i];
            }
        }
        return res;
    }

    friend bool operator==(const StringHash &lhs, const StringHash &rhs) {
        return lhs.get() == rhs.get();
    }

    friend std::strong_ordering operator<=>(const StringHash &lhs, const StringHash &rhs) {
        return lhs.get() <=> rhs.get();
    }

    template <typename Ostream>
    friend Ostream &operator<<(Ostream &ostream, const StringHash &self) {
        return ostream << self.get();
    }

private:
    std::size_t _size;
    std::vector<hash> power;
    std::vector<hash> table;
};

namespace RandomHashing {
    constexpr std::uint64_t seed() {
        std::uint64_t shifted = 0;
        for (const auto c : __TIME__ __TIMESTAMP__ __DATE__) {
            (shifted += c) *= 6364136223846793005ULL;
            shifted ^= shifted << 13;
            shifted ^= shifted >> 7;
            shifted ^= shifted << 17;
        }
        return shifted;
    }

    template <std::unsigned_integral size>
    constexpr bool MillerRabin(size value) {
        using u32 = unsigned;
        using u64 = unsigned long long;
        using u128 = unsigned __int128;
        constexpr auto digits = std::numeric_limits<size>::digits;
        using next_size = std::conditional_t<(digits < 64), u64, u128>;
        if (value < 64) {
            return 0x28208a20a08a28ac >> value & 1;
        }
        if (value % 2 == 0 or value % 3 == 0 or value % 5 == 0 or value % 7 == 0 or value % 11 == 0) {
            return false;
        }
        u32 count = std::countr_zero(value - 1);
        size u = (value - 1) >> count, inverse = size(2) - value;
        for (u32 i = digits <= 64; i < 5; ++i) {
            inverse *= size(2) - value * inverse;
        }
        auto multiply = [&value, &inverse](size lhs, size rhs) -> size {
            const auto res = next_size(lhs) * rhs;
            return value + size(res >> digits) - size((next_size(size(res) * inverse) * value) >> digits);
        };
        const size one = -value % value, value_inverse = -next_size(value) % value, value_reduce_one = value - one;
        auto prime_test = [&](std::initializer_list<size> test_base) -> bool {
            return std::all_of(test_base.begin(), test_base.end(), [&](auto base) -> bool {
                if (base >= value) {
                    return true;
                }
                size res = one;
                base = multiply(base, value_inverse);
                for (size exp = u; exp != 0; exp >>= 1) {
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

    constexpr std::size_t generate_prime(std::size_t left, std::size_t right) {
        auto shifted = seed();
        std::size_t value;
        do {
            shifted ^= shifted << 13;
            shifted ^= shifted >> 7;
            shifted ^= shifted << 17;
            value = shifted % (right - left) + left;
        } while (not MillerRabin(value));
        return value;
    }
}

constexpr std::uint64_t base = RandomHashing::generate_prime(100000000000ULL, 10000000000000ULL);
constexpr std::uint64_t modular = RandomHashing::generate_prime(100000000000000000ULL, 3000000000000000000ULL);
constexpr std::array<std::uint64_t, 1> Base = {base};
constexpr std::array<std::uint64_t, 1> Modular = {modular};
using Hashing = StringHash<1, Base, Modular>;