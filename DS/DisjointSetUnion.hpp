/**
 * @brief 并查集（Disjoint Set Union, DSU）
 * @details 支持合并集合、查询连通性、获取集合大小等操作
 *          在基础功能上增加了调试输出和集合枚举功能
 * @complexity 合并/查询: O(α(n)), 其中α为反阿克曼函数
 */
#pragma once
#include <vector>
#include <numeric>
#include <ranges>
#include <algorithm>
class DisjointSetUnion {
public:
    DisjointSetUnion(std::size_t n) : _size(n) {
        parent.resize(n);
        count.assign(n, 1);
        std::ranges::iota(parent, 0);
    }

    std::size_t size() const {
        return _size;
    }

    std::size_t find(std::size_t value) {
        return parent[value] == value ? value : parent[value] = find(parent[value]);
    }

    std::size_t size(std::size_t value) {
        return count[find(value)];
    }

    bool head(std::size_t value) const {
        return value == parent[value];
    }

    bool same(std::size_t x, std::size_t y) {
        return find(x) == find(y);
    }

    bool merge(std::size_t x, std::size_t y) {
        if ((x = find(x)) == (y = find(y))) {
            return false;
        }
        --_size;
        parent[x] = y;
        count[y] += count[x];
        return true;
    }

    void for_each(auto &&call) {
        std::vector<std::size_t> index(count.size());
        std::vector<std::vector<std::size_t>> groups(size());
        std::size_t j = 0;
        for (std::size_t j = 0; auto i : std::ranges::views::iota(std::size_t(0), count.size()) | std::ranges::views::filter([this](std::size_t x) {
            return head(x);
        })) {
            groups[index[i] = j++].reserve(count[i]);
        }
        for (std::size_t i = 0; i < count.size(); ++i) {
            groups[index[find(i)]].push_back(i);
        }
        std::ranges::for_each(groups, std::forward<decltype(call)>(call));
    }

    template <typename Ostream>
    friend Ostream &operator<<(Ostream &ostream, DisjointSetUnion &self) {
        std::size_t index = 0;
        return self.for_each([&](const auto &e) {
            if (index++ != 0) {
                ostream << "\n";
            }
            auto it = e.begin();
            for (ostream << '{' << *it++; it != e.end(); ++it) {
                ostream << ", " << *it;
            }
            ostream << '}';
        }), ostream;
    }

private:
    std::size_t _size;
    std::vector<std::size_t> parent;
    std::vector<std::size_t> count;
};