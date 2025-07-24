/**
 * @brief 线段树（Segment Tree）
 * @details 支持单点修改和区间查询的线段树，基于AtCoder库实现并重构
 *          支持多种初始化方式和二分查找功能，区间均为闭区间，使用0-based索引
 * @complexity 单点修改/区间查询: O(log n), 二分: O(log n)
 */
#pragma once
#include <bits/stdc++.h>
template <typename Info, auto op, auto e>
class SegmentTree {
private:
    using size_type = unsigned;

public:
    SegmentTree(std::integral auto n) : SegmentTree(n, [](auto...) {
        return e();
    }) {}

    SegmentTree(const std::ranges::range auto &container) : SegmentTree(std::forward<decltype(container)>(container).begin(), std::forward<decltype(container)>(container).end()) {}

    template <typename Iterator>
    SegmentTree(Iterator begin, Iterator end) : SegmentTree(end - begin, [&](const auto &index) {
        return *(begin + index);
    }) {}

    SegmentTree(std::integral auto n, auto &&mapping) : max_range(n) {
        if (max_range == 0) {
            return;
        }
        ceil_size = std::bit_ceil(max_range);
        ceil_log = std::countr_zero(ceil_size);
        tree.resize(ceil_size << 1, e());
        for (size_type i = 0; i < max_range; ++i) {
            tree[ceil_size + i] = std::forward<decltype(mapping)>(mapping)(i);
        }
        for (size_type i = ceil_size - 1; i > 0; --i) {
            update(i);
        }
    }

    Info prod_all() const {
        return tree[1];
    }

    Info get(size_type position) const {
        return tree[position + ceil_size];
    }

    void set(size_type position, Info value) {
        position += ceil_size;
        tree[position] = value;
        for (size_type i = 1; i <= ceil_log; ++i) {
            update(position >> i);
        }
    }

    Info prod(size_type left, size_type right) const {
        Info ls = e(), rs = e();
        for (left += ceil_size, right += ceil_size + 1; left < right; left >>= 1, right >>= 1) {
            if (left & 1) {
                ls = op(ls, tree[left++]);
            }
            if (right & 1) {
                rs = op(tree[--right], rs);
            }
        }
        return op(ls, rs);
    }

    size_type min_left(size_type right, auto &&check) const {
        if (right < 0 or right >= max_range or not check(get(right))) {
            return -1;
        }
        return right == 0 ? right : binary_search(right, -1, [&](auto left) {
            return check(prod(left, right));
        });
    }

    size_type max_right(size_type left, auto &&check) const {
        if (left < 0 or left >= max_range or not check(get(left))) {
            return -1;
        }
        return left == max_range - 1 ? left : binary_search(left, max_range, [&](auto right) {
            return check(prod(left, right));
        });
    }

private:
    std::vector<Info> tree;
    size_type max_range, ceil_size, ceil_log;
    void update(size_type index) {
        tree[index] = op(tree[index << 1], tree[index << 1 | 1]);
    }

    size_type binary_search(int ok, int ng, auto &&check) const {
        for (int x; std::abs(ok - ng) > 1; (std::forward<decltype(check)>(check)(x) ? ok : ng) = x) {
            x = ok + (ng - ok) / 2;
        }
        return ok;
    }
};