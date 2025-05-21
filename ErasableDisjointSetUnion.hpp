#pragma once
#include <vector>
#include <numeric>

class ErasableDisjointSetUnion {
public:
    constexpr ErasableDisjointSetUnion(std::size_t n) : max(n), group(n) {
        parent.resize(2 * n);
        _size.assign(n, 1);
        std::iota(parent.begin(), parent.begin() + n, n);
        std::iota(parent.begin() + n, parent.end(), n);
    }

    constexpr auto size() const -> std::size_t {
        return group;
    }

    constexpr auto size(std::size_t value) -> std::size_t {
        return _size[find(value)];
    }

    constexpr auto find(std::size_t value) -> std::size_t {
        auto _find = [&](auto &&self, std::size_t value) -> std::size_t {
            return parent[value] == value ? value : parent[value] = self(self, parent[value]);
        };
        return _find(_find, value) - max;
    }

    constexpr auto extract(std::size_t value) -> std::size_t {
        auto head = find(value);
        if (_size[head] == 1) {
            return head;
        }
        parent[value] = parent.size();
        parent.push_back(parent.size());
        --_size[head];
        _size.push_back(1);
        ++group;
        return parent[value] - max;
    }

    constexpr auto same(std::size_t x, std::size_t y) -> bool {
        return find(x) == find(y);
    }

    constexpr auto merge(std::size_t x, std::size_t y) -> bool {
        if ((x = find(x)) == (y = find(y))) {
            return false;
        }
        --group;
        parent[max + x] = max + y;
        _size[y] += _size[x];
        _size[x] = 0;
        return true;
    }

private:
    std::size_t max;
    std::size_t group;
    std::vector<std::size_t> parent;
    std::vector<std::size_t> _size;
};