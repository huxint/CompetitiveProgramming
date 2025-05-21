#pragma once
#include <bits/stdc++.h>

template <typename Info, auto op, auto e>
class SegmentTree {
private:
    using size_type = unsigned;

public:
    constexpr SegmentTree(std::integral auto n) : SegmentTree(n, [](auto...) {
        return e();
    }) {}

    constexpr SegmentTree(const std::ranges::range auto &container) : SegmentTree(container.begin(), container.end()) {}

    template <typename Iterator>
    constexpr SegmentTree(Iterator begin, Iterator end) : SegmentTree(end - begin, [&](const auto &index) {
        return *(begin + index);
    }) {}

    constexpr SegmentTree(std::integral auto n, auto &&mapping) : max_range(n) {
        if (max_range == 0) {
            return;
        }
        ceil_size = std::bit_ceil(max_range);
        ceil_log = std::countr_zero(ceil_size);
        tree.resize(ceil_size << 1, e());
        for (size_type i = 0; i < max_range; ++i) {
            tree[ceil_size + i] = mapping(i);
        }
        for (size_type i = ceil_size - 1; i > 0; --i) {
            update(i);
        }
    }

    constexpr auto prod_all() const -> Info {
        return tree[1];
    }

    constexpr auto get(size_type position) const -> Info {
        return tree[position + ceil_size];
    }

    constexpr auto set(size_type position, Info value) -> void {
        position += ceil_size;
        tree[position] = value;
        for (size_type i = 1; i <= ceil_log; ++i) {
            update(position >> i);
        }
    }

    constexpr auto prod(size_type left, size_type right) const -> Info {
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

    constexpr auto min_left(size_type right, auto &&check) const -> size_type {
        if (right < 0 or right >= max_range or not check(get(right))) {
            return -1;
        }
        return right == 0 ? right : binary_search(right, -1, [&](auto left) {
            return check(prod(left, right));
        });
    }

    constexpr auto max_right(size_type left, auto &&check) const -> size_type {
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
    constexpr auto update(size_type index) -> void {
        tree[index] = op(tree[index << 1], tree[index << 1 | 1]);
    }

    constexpr auto binary_search(int ok, int ng, auto &&check) const -> size_type {
        for (int x; std::abs(ok - ng) > 1; (check(x) ? ok : ng) = x) {
            x = ok + (ng - ok) / 2;
        }
        return ok;
    }
};