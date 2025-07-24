/**
 * @brief 离散化工具类
 * @details 将连续的值域映射到较小的离散区间，常用于坐标压缩
 *          支持查询原值对应的离散化位置和离散化位置对应的原值
 * @complexity 预处理: O(n log n), 查询: O(log n)
 */
#pragma once
#include <vector>
#include <algorithm>
template <typename T>
class Discretizer {
public:
    Discretizer() : _prepared{}, discretizer{} {}

    Discretizer(std::ranges::range auto container) : Discretizer() {
        discretizer.reserve(container.size());
        std::copy(container.begin(), container.end(), std::back_inserter(discretizer));
    }

    template <typename Iterator>
    Discretizer(Iterator begin, Iterator end) : Discretizer() {
        discretizer.reserve(end - begin);
        std::copy(begin, end, std::back_inserter(discretizer));
    }

    Discretizer(std::size_t size, auto &&mapping) : Discretizer() {
        discretizer.reserve(size);
        for (std::size_t i = 0; i < size; ++i) {
            discretizer.push_back(std::forward<decltype(mapping)>(mapping)(i));
        }
    }

    void push(const T &value) {
        if (not _prepared) {
            discretizer.push_back(value);
            return;
        }
        auto it = std::lower_bound(discretizer.begin(), discretizer.end(), value);
        if (it == discretizer.end()) {
            discretizer.push_back(value);
        } else {
            if (*it != value) {
                discretizer.insert(it, value);
            }
        }
    }

    std::size_t size() const {
        return discretizer.size();
    }

    void prepare() {
        _prepared = true;
        std::sort(discretizer.begin(), discretizer.end());
        discretizer.resize(std::unique(discretizer.begin(), discretizer.end()) - discretizer.begin());
    }

    std::size_t rank(const T &value) const {
        return std::distance(discretizer.begin(), std::lower_bound(discretizer.begin(), discretizer.end(), value));
    }

    template <typename Ostream>
    friend Ostream &operator<<(Ostream &ostream, const Discretizer &self) {
        auto it = self.discretizer.begin();
        for (ostream << *it++; it != self.discretizer.end(); ++it) {
            ostream << ' ' << *it;
        }
        return ostream;
    }

private:
    bool _prepared;
    std::vector<T> discretizer;
};