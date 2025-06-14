#pragma once
#include "bit.hpp"
#include "types.hpp"

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