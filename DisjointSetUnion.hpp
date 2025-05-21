#pragma once
#include <vector>
#include <numeric>

class DisjointSetUnion {
public:
    DisjointSetUnion(std::size_t n) : _size(n), _total(n) {
        parent.resize(n);
        count.assign(n, 1);
        std::iota(parent.begin(), parent.end(), 0);
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
        std::vector<std::size_t> index(_total);
        std::vector<std::vector<std::size_t>> groups(size());
        for (std::size_t i = 0, j = 0; i < _total; ++i) {
            if (not head(i)) {
                continue;
            }
            groups[index[i] = j++].reserve(count[i]);
        }
        for (std::size_t i = 0; i < _total; ++i) {
            groups[index[find(i)]].push_back(i);
        }
        std::for_each(groups.begin(), groups.end(), std::forward<decltype(call)>(call));
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
    std::size_t _total;
    std::vector<std::size_t> parent;
    std::vector<std::size_t> count;
};