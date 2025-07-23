#pragma once
#include <bit>
#include <array>
#include <vector>
#include <ranges>
#include <type_traits>

/*
树状数组
只支持维护可差分的信息
如 加法， 乘法， 异或

你需要重载 operator +, +=, -, *(只有区间修改需要 倍增) 来实现其他操作，默认是加法
*/
template <typename T, bool range = false>
class BinaryIndexedTree {
private:
    struct Container {
        std::array<T, 2> value;
        Container() : value{} {}

        Container(T a, T b) : value{a, b} {}

        Container &operator+=(const Container &other) {
            value[0] += other.value[0];
            value[1] += other.value[1];
            return *this;
        }
    };
    using node = std::conditional_t<range, Container, T>;

public:
    BinaryIndexedTree() : max{} {}

    BinaryIndexedTree(std::size_t length) : max{length ? std::bit_ceil(length) : 0} {
        tree.assign(max, node{});
    }

    BinaryIndexedTree(const std::ranges::range auto &container) : BinaryIndexedTree(std::forward<decltype(container)>(container).begin(), std::forward<decltype(container)>(container).end()) {}

    template <typename Iterator>
    BinaryIndexedTree(Iterator begin, Iterator end) : BinaryIndexedTree(end - begin, [&](const auto &index) {
        return *(begin + index);
    }) {}

    BinaryIndexedTree(std::size_t length, auto &&mapping) : BinaryIndexedTree(length) {
        if constexpr (range) {
            T previous{};
            for (std::size_t i = 0; i < length or (length != max and i == length); ++i) {
                T current{i != length ? std::forward<decltype(mapping)>(mapping)(i) : T{}};
                tree[i].value[0] = current - previous;
                tree[i].value[1] = tree[i].value[0] * i;
                previous = current;
            }
        } else {
            for (std::size_t i = 0; i < length; ++i) {
                tree[i] = std::forward<decltype(mapping)>(mapping)(i);
            }
        }
        for (std::size_t i = 0, j = 1; i < max and j < max; ++i, j = i + lowbit(i + 1)) {
            tree[j] += tree[i];
        }
    }

    void add(std::size_t index, const T &increase) {
        if constexpr (range) {
            add_impl(index, node(increase, T(increase * index)));
            add_impl(index + 1, node(-increase, T(-increase * (index + 1))));
        } else {
            add_impl(index, increase);
        }
    }

    void add(std::size_t left, std::size_t right, const T &increase) {
        add_impl(left, node(increase, T(increase * left)));
        add_impl(right + 1, node(-increase, T(-increase * (right + 1))));
    }

    T query() const {
        return prefix(max - 1);
    }

    T query(std::size_t left, std::size_t right) const {
        if (left == 0) {
            return prefix(right);
        } else {
            return prefix(right) - prefix(left - 1);
        }
    }

    T query(std::size_t index) const {
        T res{};
        if constexpr (range) {
            for (std::size_t i = index; ~i; i -= lowbit(i + 1)) {
                res += tree[i].value[0];
            }
        } else {
            res = tree[index];
            for (std::size_t i = index - 1, j = lowbit(index + 1); j >>= 1; i -= lowbit(i + 1)) {
                res -= tree[i];
            }
        }
        return res;
    }

    std::size_t kth(T value) const {
        int index = -1;
        if constexpr (range) {
            Container current{};
            const auto get{[](const Container &lhs, const Container &rhs, int index) {
                return (lhs.value[0] + rhs.value[0]) * (index + 1) - (lhs.value[1] + rhs.value[1]);
            }};
            for (std::size_t d = max >> 1; d != 0; d >>= 1) {
                if (get(current, tree[index + d], index) <= value) {
                    index += d;
                    current += tree[index];
                }
            }
        } else {
            for (std::size_t d = max >> 1; d != 0; d >>= 1) {
                if (tree[index + d] <= value) {
                    index += d;
                    value -= tree[index];
                }
            }
        }
        return index + 1;
    }

    void for_each(auto &&call) const {
        if constexpr (range) {
            T val{};
            for (std::size_t i = 0; i < max; ++i) {
                val += tree[i].value[0];
                for (std::size_t ctz = std::countr_zero(~i); ctz--;) {
                    val -= tree[i - (static_cast<std::size_t>(1) << ctz)].value[0];
                }
                std::forward<decltype(call)>(call)(val);
            }
        } else {
            for (std::size_t i = 0; i < max; ++i) {
                std::forward<decltype(call)>(call)(query(i));
            }
        }
    }

private:
    std::size_t max;
    std::vector<node> tree;
    static constexpr std::size_t lowbit(std::size_t x) {
        return x & -x;
    }

    void add_impl(std::size_t index, const node &increase) {
        while (index < max) {
            tree[index] += increase;
            index += lowbit(index + 1);
        }
    }

    T prefix(std::size_t index) const {
        node res{};
        for (std::size_t i = index; ~i; i -= lowbit(i + 1)) {
            res += tree[i];
        }
        if constexpr (range) {
            return res.value[0] * (index + 1) - res.value[1];
        } else {
            return res;
        }
    }
};