/**
 * @brief 搜索算法集合
 * @details 提供二分搜索和三分搜索的通用实现
 *          支持整数和浮点数的二分搜索，以及浮点数的三分搜索（求极值）
 * @complexity 二分搜索: O(log n), 三分搜索: O(log n)
 */
#pragma once
#include <bits/stdc++.h>

auto bisect_search(std::integral auto ok, std::integral auto ng, auto &&check) {
    for (decltype(ok) x; std::abs(ok - ng) > 1; (check(x) ? ok : ng) = x) {
        x = ok + (ng - ok) / 2;
    }
    return ok;
}

auto bisect_search(std::floating_point auto ok, std::floating_point auto ng, auto &&check, std::size_t loop = 300) {
    for (decltype(ok) x; loop--; (check(x) ? ok : ng) = x) {
        x = ok + (ng - ok) / 2;
    }
    return ok;
}

auto ternary_search_max(std::floating_point auto left, std::floating_point auto right, auto &&calculate, std::size_t loop = 300) {
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

auto ternary_search_min(std::floating_point auto left, std::floating_point auto right, auto &&calculate, std::size_t loop = 300) {
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

auto ternary_search_max(std::integral auto left, std::integral auto right, auto &&calculate) {
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

auto ternary_search_min(std::integral auto left, std::integral auto right, auto &&calculate) {
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