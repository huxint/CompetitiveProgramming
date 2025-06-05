#pragma once

#include <map>
#include <set>
#include <array>
#include <cmath>
#include <tuple>
#include <queue>
#include <bitset>
#include <vector>
#include <ranges>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <numeric>
#include <concepts>
#include <iostream>
#include <algorithm>

using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;
constexpr auto inf32 = std::numeric_limits<i32>::max() / 2;
constexpr auto inf64 = std::numeric_limits<i64>::max() / 2;

template <typename... Args>
bool cmax(auto &value, const Args &...args) {
    if (const auto other = std::max({static_cast<std::common_type_t<Args...>>(args)...}); value < other) {
        value = other;
        return true;
    }
    return false;
}

template <typename... Args>
bool cmin(auto &value, const Args &...args) {
    if (const auto other = std::min({static_cast<std::common_type_t<Args...>>(args)...}); value > other) {
        value = other;
        return true;
    }
    return false;
}

auto bisect_search(std::integral auto ok, std::integral auto ng, auto &&check) {
    for (decltype(ok) x; std::abs(ok - ng) > 1; (std::forward<decltype(check)>(check)(x) ? ok : ng) = x) {
        x = ok + (ng - ok) / 2;
    }
    return ok;
}

auto bisect_search(std::floating_point auto ok, std::floating_point auto ng, auto &&check, std::size_t loop = 30) {
    for (decltype(ok) x; loop--; (std::forward<decltype(check)>(check)(x) ? ok : ng) = x) {
        x = ok + (ng - ok) / 2;
    }
    return ok;
}

namespace iostream {
    template <typename T>
    concept range_like = std::ranges::range<T> and not std::is_convertible_v<T, std::string_view>;
    template <typename T>
    concept range_tuple = range_like<T> or std::__is_tuple_like<T>::value and not std::ranges::range<T>;

    template <typename Istream>
    Istream &operator>>(Istream &istream, range_tuple auto &value) {
        if constexpr (range_like<decltype(value)>) {
            for (auto &e : value) {
                istream >> e;
            }
        } else {
            std::apply([&istream](auto &&...args) {
                ((istream >> args), ...);
            }, value);
        }
        return istream;
    }

    template <typename Ostream>
    Ostream &operator<<(Ostream &ostream, const range_tuple auto &value) {
        if constexpr (range_like<decltype(value)>) {
            auto it = value.begin();
            for (ostream << *it++; it != value.end(); ++it) {
                ostream << ' ' << *it;
            }
        } else {
            ostream << '(' << (std::apply([&ostream](auto &&...args) {
                auto size = sizeof...(args);
                ((ostream << args << (--size ? ", " : "")), ...);
            }, value), ')');
        }
        return ostream;
    }
}
using iostream::operator<<, iostream::operator>>;

namespace debug {
    void dbg(const char *name, auto &&arg) {
        std::cerr << '{' << name << ": " << arg << "}\n";
    }

    void dbg(const char *names, auto &&arg, auto &&...args) {
        const char *comma = std::strchr(names + 1, ',');
        std::cerr << '{';
        std::cerr.write(names, comma - names) << ": " << arg << "}, ";
        dbg(comma + 2, args...);
    }
}
#define debug(...) debug::dbg(#__VA_ARGS__, __VA_ARGS__)

template <typename Istream, typename Ostream>
int huxint(Istream &&istream, Ostream &&ostream, bool multiple_test, std::size_t precision, auto &&solve) {
    if constexpr (std::is_base_of_v<std::remove_reference_t<Istream>, std::istream> and std::is_base_of_v<std::remove_reference_t<Ostream>, std::ostream>) {
        std::ios_base::sync_with_stdio(false);
        istream.tie(nullptr);
        ostream << std::fixed << std::setprecision(precision);
    }
    std::size_t testcases = 1;
    if (multiple_test) {
        istream >> testcases;
    }
    while (testcases--) {
        solve();
    }
    return 0;
};