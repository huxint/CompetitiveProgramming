#pragma once
#include <bit>
#include <array>
#include <limits>

/*
线性基

支持插入、查询、枚举、合并

支持求最大值、最小值、第 k 大值

支持求 rank
*/
template <std::unsigned_integral T>
class XorBase {
private:
    static constexpr auto digits = std::numeric_limits<T>::digits;

public:
    XorBase() {
        basic.fill(0);
    }

    XorBase(T value) : XorBase() {
        insert(value);
    }

    XorBase(std::size_t n, auto &&mapping) : XorBase() {
        for (std::size_t i = 0; i != n; ++i) {
            insert(mapping(i));
        }
    }

    template <typename Iterator>
    XorBase(Iterator first, Iterator last) : XorBase() {
        for (; first != last; ++first) {
            insert(*first);
        }
    }

    T kth(std::size_t k, T base = 0) const {
        T ans = base;
        for (std::size_t i = digits - 1, all = std::size_t(1) << base_size(); all > 1 and ~i; --i) {
            if (basic[i] == 0) {
                continue;
            }
            all >>= 1;
            if (k >= all) {
                if ((ans ^ basic[i]) > ans) {
                    ans ^= basic[i];
                }
                k -= all;
            } else if ((ans ^ basic[i]) < ans) {
                ans ^= basic[i];
            }
        }
        return ans;
    }

    T rank(T value) const {
        T ans = 0;
        for (std::size_t i = digits - 1, all = std::size_t(1) << base_size(); ~i; --i) {
            if (basic[i] == 0) {
                continue;
            }
            all >>= 1;
            if (value >> i & 1) {
                ans += all;
            }
        }
        return ans;
    }

    std::size_t insert(T value) {
        for (std::size_t i = std::bit_width(value) - 1; value != 0 and ~i; --i) {
            if (value >> i & 1) {
                if (basic[i] == 0) {
                    basic[i] = value;
                    return i;
                } else {
                    value ^= basic[i];
                }
            }
        }
        return -1;
    }

    bool contains(T value) const {
        for (std::size_t i = std::bit_width(value) - 1; value != 0 and ~i; --i) {
            if (basic[i] != 0 and (value >> i & 1)) {
                value ^= basic[i];
            }
        }
        return value == 0;
    }

    std::size_t base_size() const {
        return std::count_if(basic.begin(), basic.end(), [](auto x) {
            return x != 0;
        });
    }

    void enumerate(auto &&call) const {
        std::vector<std::size_t> next(digits + 1, -1);
        for (std::size_t last = digits, i = digits - 1; ~i; --i) {
            if (basic[i] != 0) {
                last = next[last] = i;
            }
        }
        auto dfs = [&](auto &&self, std::size_t index, T value, auto &&call) -> void {
            if (std::size_t i = next[index]; ~i) {
                self(self, i, value, call);
                self(self, i, value ^ basic[i], call);
            } else {
                call(value);
            }
        };
        dfs(dfs, digits, 0, call);
    }

    void enumerate_base(auto &&call) const {
        for (std::size_t i = digits - 1; ~i; --i) {
            if (basic[i] != 0) {
                call(basic[i]);
            }
        }
    }

    T max(T base = 0) const {
        T res = base;
        for (std::size_t i = digits - 1; ~i; --i) {
            if ((res ^ basic[i]) > res) {
                res ^= basic[i];
            }
        }
        return res;
    }

    T min(T base = 0) const {
        return kth(0, base);
    }

    XorBase &operator+=(const XorBase &other) {
        for (std::size_t i = 0; i != digits; ++i) {
            if (other.basic[i] != 0) {
                insert(other.basic[i]);
            }
        }
        return *this;
    }

    friend XorBase operator+(const XorBase &lhs, const XorBase &rhs) {
        return XorBase(lhs) += rhs;
    }

private:
    std::array<T, digits> basic;
};