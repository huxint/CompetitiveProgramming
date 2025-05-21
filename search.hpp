#pragma once
#include <bits/stdc++.h>

constexpr auto bisect_search(std::integral auto ok, std::integral auto ng, auto &&check) -> decltype(ok) {
    for (decltype(ok) x; std::abs(ok - ng) > 1; (check(x) ? ok : ng) = x) {
        x = ok + (ng - ok) / 2;
    }
    return ok;
}

constexpr auto bisect_search(std::floating_point auto ok, std::floating_point auto ng, auto &&check, std::size_t loop = 300) -> decltype(ok) {
    for (decltype(ok) x; loop--; (check(x) ? ok : ng) = x) {
        x = ok + (ng - ok) / 2;
    }
    return ok;
}

constexpr auto ternary_search_max(std::floating_point auto left, std::floating_point auto right, auto &&calculate, std::size_t loop = 300) -> decltype(left) {
    while (loop--) {
        auto left_mid = left + (right - left) / 3;
        auto right_mid = right - (right - left) / 3;
        if (calculate(left_mid) <= calculate(right_mid)) {
            left = left_mid;
        } else {
            right = right_mid;
        }
    }
    return std::max(calculate(left), calculate(right));
}

constexpr auto ternary_search_min(std::floating_point auto left, std::floating_point auto right, auto &&calculate, std::size_t loop = 300) -> decltype(left) {
    while (loop--) {
        auto left_mid = left + (right - left) / 3;
        auto right_mid = right - (right - left) / 3;
        if (calculate(left_mid) <= calculate(right_mid)) {
            right = right_mid;
        } else {
            left = left_mid;
        }
    }
    return std::min(calculate(left), calculate(right));
}

constexpr auto ternary_search_max(std::integral auto left, std::integral auto right, auto &&calculate) -> decltype(left) {
    while (std::abs(left - right) > 1) {
        auto left_mid = left + (right - left) / 3;
        auto right_mid = right - (right - left) / 3;
        if (calculate(left_mid) <= calculate(right_mid)) {
            left = left_mid + 1;
        } else {
            right = right_mid - 1;
        }
    }
    return std::max(calculate(left), calculate(right));
}

constexpr auto ternary_search_min(std::integral auto left, std::integral auto right, auto &&calculate) -> decltype(left) {
    while (std::abs(left - right) > 1) {
        auto left_mid = left + (right - left) / 3;
        auto right_mid = right - (right - left) / 3;
        if (calculate(left_mid) <= calculate(right_mid)) {
            right = right_mid - 1;
        } else {
            left = left_mid + 1;
        }
    }
    return std::min(calculate(left), calculate(right));
}