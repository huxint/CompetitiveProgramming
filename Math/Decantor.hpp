#pragma once
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <vector>

template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
std::vector<T> decantor(Iterator begin, Iterator end, std::uint64_t k) {
    std::vector<T> container;
    container.reserve(end - begin);
    std::copy(begin, end, std::back_inserter(container));
    std::sort(container.begin(), container.end());
    container.erase(std::unique(container.begin(), container.end()), container.end());
    std::vector<std::size_t> counter(container.size());
    for (auto it = begin; it != end; ++it) {
        ++counter[std::lower_bound(container.begin(), container.end(), *it) - container.begin()];
    }

    std::uint64_t permutation = 1, number = 0;
    for (auto e : counter) {
        for (std::size_t i = 1; i <= e; ++i) {
            permutation *= ++number;
            permutation /= i;
        }
    }

    std::vector<T> res;
    if (permutation <= k) {
        return res;
    }

    res.reserve(end - begin);
    for (std::size_t i = 0, size = end - begin; i < size; ++i) {
        std::uint64_t sum = 0;
        auto idx = std::find_if(counter.begin(), counter.end(), [&](auto count) {
            auto s = permutation * count / (size - i);
            if (sum + s > k) {
                permutation = s;
                return true;
            }
            sum += s;
            return false;
        }) - counter.begin();

        k -= sum;
        --counter[idx];
        res.push_back(container[idx]);
    }
    return res;
}