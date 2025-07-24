/**
 * @brief ST表（Sparse Table）
 * @details 支持RMQ（区间最值查询）等可重复贡献问题的数据结构
 *          支持多种初始化方式，操作函数需满足可重复贡献性质（如max、min、gcd等）
 *          使用示例：SparseTable<int, std::ranges::max>、SparseTable<int, std::ranges::min>、SparseTable<int, std::gcd<int, int>>等等
 * @complexity 预处理: O(n log n), 查询: O(1)
 */
#pragma once
#include <bit>
#include <vector>
#include <ranges>
#include <cstddef>
#include <utility>
#include <concepts>
#include <functional>
template <typename T, auto op, std::size_t level = 23>
class SparseTable {
public:
    SparseTable(std::size_t n) : SparseTable(n, [](auto ...) {
        return T{};
    }) {}

    SparseTable(const std::ranges::range auto &container) : SparseTable(std::forward<decltype(container)>(container).begin(), std::forward<decltype(container)>(container).end()) {}

    template <typename Iterator>
    SparseTable(Iterator begin, Iterator end) : SparseTable(end - begin, [&](const auto &index) {
        return *(begin + index);
    }) {}

    template <typename Mapping>
        requires std::invocable<Mapping, std::size_t>
    SparseTable(std::size_t n, Mapping &&mapping) : _size(n) {
        if (_size == 0) {
            return;
        }
        int depth = _size == 1 ? 1 : std::bit_width(_size - 1);
        table[0].resize(_size);
        for (std::size_t i = 0; i < _size; ++i) {
            table[0][i] = std::forward<Mapping>(mapping)(i);
        }
        for (int i = 1; i < depth; ++i) {
            std::size_t log_size = _size - (1 << i) + 1;
            table[i].resize(log_size);
            for (std::size_t start = 0, end = 1 << (i - 1); start < log_size; ++start, ++end) {
                table[i][start] = op(table[i - 1][start], table[i - 1][end]);
            }
        }
    }

    std::size_t size() const {
        return _size;
    }

    T query() const {
        return query(0, _size - 1);
    }

    T query(std::size_t index) const {
        return table[0][index];
    }

    T query(std::size_t left, std::size_t right) const {
        std::size_t depth = std::bit_width((right - left) >> 1);
        return op(table[depth][left], table[depth][right - (1 << depth) + 1]);
    }

    std::size_t min_left(std::size_t right, auto &&check) const {
        if (right < 0 or right >= _size or not check(query(right))) {
            return -1;
        }
        return right == 0 ? right : binary_search(right, -1, [&](auto left) {
            return check(query(left, right));
        });
    }

    std::size_t max_right(std::size_t left, auto &&check) const {
        if (left < 0 or left >= _size or not check(query(left))) {
            return -1;
        }
        return left == _size - 1 ? left : binary_search(left, _size, [&](auto right) {
            return check(query(left, right));
        });
    }

    template <typename Ostream>
    friend Ostream &operator<<(Ostream &ostream, const SparseTable &value) {
        for (std::size_t i = 0; i + 1 < value.size(); ++i) {
            ostream << value.query(i) << ' ';
        }
        return ostream << value.query(value.size() - 1);
    }

private:
    std::size_t _size;
    std::array<std::vector<T>, level> table;
    std::size_t binary_search(int ok, int ng, auto &&check) const {
        for (int x; std::abs(ok - ng) > 1; (check(x) ? ok : ng) = x) {
            x = ok + (ng - ok) / 2;
        }
        return ok;
    }
};