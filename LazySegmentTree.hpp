#pragma once
#include <bits/stdc++.h>

template <typename Info, typename Function, auto Mapping, auto Composition, auto op, auto e, auto id>
class LazySegmentTree {
private:
    using size_type = unsigned;

public:
    constexpr LazySegmentTree(std::integral auto n) : LazySegmentTree(n, [](auto...) {
        return e();
    }) {}

    constexpr LazySegmentTree(const std::ranges::range auto &container) : LazySegmentTree(container.begin(), container.end()) {}

    template <typename Iterator>
    constexpr LazySegmentTree(Iterator begin, Iterator end) : LazySegmentTree(end - begin, [&](const auto &index) {
        return *(begin + index);
    }) {}

    constexpr LazySegmentTree(std::integral auto n, auto &&mapping) : max_range(n) {
        if (max_range == 0) {
            return;
        }
        ceil_size = std::bit_ceil(max_range);
        ceil_log = std::countr_zero(ceil_size);
        tree = std::vector<Info>(ceil_size << 1, e());
        lazy = std::vector<Function>(ceil_size, id());
        for (size_type index = 0; index < max_range; ++index) {
            tree[ceil_size + index] = mapping(index);
        }
        for (size_type index = ceil_size - 1; index > 0; --index) {
            update(index);
        }
    }

    constexpr auto prod_all() -> Info {
        return tree[1];
    }

    constexpr auto get(size_type position) -> Info {
        position += ceil_size;
        for (size_type index = ceil_log; index > 0; --index) {
            push(position >> index);
        }
        return tree[position];
    }

    constexpr auto set(size_type position, Info value) -> void {
        position += ceil_size;
        for (size_type index = ceil_log; index > 0; --index) {
            push(position >> index);
        }
        tree[position] = value;
        for (size_type index = 1; index <= ceil_log; ++index) {
            update(position >> index);
        }
    }

    constexpr auto prod(size_type left, size_type right) -> Info {
        left += ceil_size, right += ceil_size + 1;
        for (size_type index = ceil_log; index > 0; --index) {
            if (((left >> index) << index) != left) {
                push(left >> index);
            }
            if (((right >> index) << index) != right) {
                push((right - 1) >> index);
            }
        }
        Info ls = e(), rs = e();
        for (; left < right; left >>= 1, right >>= 1) {
            if (left & 1) {
                ls = op(ls, tree[left++]);
            }
            if (right & 1) {
                rs = op(tree[--right], rs);
            }
        }
        return op(ls, rs);
    }

    constexpr auto apply(size_type position, Function function) -> void {
        position += ceil_size;
        for (size_type index = ceil_log; index > 0; --index) {
            push(position >> index);
        }
        tree[position] = Mapping(function, tree[position]);
        for (size_type index = 1; index <= ceil_log; ++index) {
            update(position >> index);
        }
    }

    constexpr auto apply(size_type left, size_type right, Function function) -> void {
        left += ceil_size, right += ceil_size + 1;
        for (size_type index = ceil_log; index > 0; --index) {
            if (((left >> index) << index) != left) {
                push(left >> index);
            }
            if (((right >> index) << index) != right) {
                push((right - 1) >> index);
            }
        }
        for (size_type _left = left, _right = right; _left < _right; _left >>= 1, _right >>= 1) {
            if (_left & 1) {
                apply_all(_left++, function);
            }
            if (_right & 1) {
                apply_all(--_right, function);
            }
        }
        for (size_type index = 1; index <= ceil_log; ++index) {
            if (((left >> index) << index) != left) {
                update(left >> index);
            }
            if (((right >> index) << index) != right) {
                update((right - 1) >> index);
            }
        }
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
    std::vector<Function> lazy;
    size_type max_range, ceil_size, ceil_log;
    constexpr auto update(size_type index) -> void {
        tree[index] = op(tree[index << 1], tree[index << 1 | 1]);
    }

    constexpr auto push(size_type index) -> void {
        apply_all(index << 1, lazy[index]);
        apply_all(index << 1 | 1, lazy[index]);
        lazy[index] = id();
    }

    constexpr auto apply_all(size_type index, Function function) -> void {
        tree[index] = Mapping(function, tree[index]);
        if (index < ceil_size) {
            lazy[index] = Composition(function, lazy[index]);
        }
    }

    constexpr auto binary_search(int ok, int ng, auto &&check) -> size_type {
        for (int x; std::abs(ok - ng) > 1; (check(x) ? ok : ng) = x) {
            x = ok + (ng - ok) / 2;
        }
        return ok;
    }
};