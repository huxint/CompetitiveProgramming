#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm>

/*
马拉车
线性求最长回文串长度
*/
class Manacher {
public:
    Manacher() {}

    Manacher(std::string_view str) : Manacher(str.begin(), str.end()) {}

    template <typename Iterator>
    Manacher(Iterator begin, Iterator end) {
        _size = end - begin;

        odd.clear(), odd.reserve(size());
        even.clear(), even.reserve(size());

        std::int32_t l1 = -1, r1 = -1;
        std::int32_t l2 = 0, r2 = -1;

        auto mapping = [&](std::int32_t i) {
            return *(begin + i);
        };

        for (std::int32_t i = 0; i != static_cast<std::int32_t>(size()); ++i) {
            std::int32_t k1 = i <= r1 ? std::min(odd[l1 + r1 - i], r1 - i) + 1 : 1;
            std::int32_t k2 = i <= r2 ? std::min(even[l2 + r2 - i + 1], r2 - i + 1) + 1 : 1;

            while (k1 <= i and k1 + i < static_cast<std::int32_t>(size()) and mapping(i - k1) == mapping(i + k1)) {
                ++k1;
            }

            while (k2 <= i and k2 + i < static_cast<std::int32_t>(size()) + 1 and mapping(i - k2) == mapping(i + k2 - 1)) {
                ++k2;
            }

            odd.push_back(k1 - 1);
            even.push_back(k2 - 1);

            if (i + k1 > r1 + 1) {
                l1 = i - k1 + 1;
                r1 = i + k1 - 1;
            }

            if (i + k2 > r2 + 2) {
                l2 = i - k2 + 1;
                r2 = i + k2 - 2;
            }
        }
    }

    std::size_t size() const {
        return _size;
    }

    bool check(std::size_t l, std::size_t r) const {
        return (r - l) % 2 == 0 ? odd[(l + r) / 2] >= (r - l) / 2 : even[(l + r + 1) / 2] > (r - l) / 2;
    }

    std::int32_t max() const {
        return std::max(2 * *std::max_element(odd.begin(), odd.end()) + 1, 2 * *std::max_element(even.begin(), even.end()));
    }

    std::int32_t query_odd(std::size_t center) const {
        return odd[center];
    }

    std::int32_t query_even(std::size_t l, std::size_t r) const {
        assert(r - l == 1);
        return even[r];
    }

private:
    std::size_t _size;
    std::vector<std::int32_t> odd;
    std::vector<std::int32_t> even;
};