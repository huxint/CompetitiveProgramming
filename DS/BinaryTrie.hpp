/**
 * @brief 01字典树（Binary Trie）
 * @details 基于二进制位的字典树，支持插入、删除、计数、最大最小值查询、枚举等操作
 *          适用于处理整数的位运算相关问题，如异或最大值、异或第k小等
 * @complexity 插入/删除/查询: O(log U), 其中U为值域大小
 */
#pragma once
#include <array>
#include <vector>
#include <cstdint>
#include <utility>
#include <algorithm>
template <std::unsigned_integral Key, typename T = std::size_t, std::size_t width = std::numeric_limits<Key>::digits>
class BinaryTrie {
private:
    using node = std::pair<T, std::array<std::size_t, 2>>;
    static constexpr auto root = std::size_t(0);
    static constexpr auto invalid = std::size_t(-1);
    static constexpr auto children = std::array<std::size_t, 2>{invalid, invalid};

public:
    BinaryTrie() : _size() {
        allocate_node();
    }

    void clear() {
        _size = 0;
        trie.clear();
        allocate_node();
    }

    T size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    void insert(Key key, T count = 1) {
        if (count <= 0) {
            return;
        }
        std::size_t index = root;
        trie.at(root).first += count;
        for (std::size_t i = width - 1; i + 1 != 0; --i) {
            Key bit = key >> i & 1;
            if (trie.at(index).second.at(bit) == invalid) {
                allocate_node();
                trie.at(index).second.at(bit) = trie.size() - 1;
            }
            trie.at(index = trie.at(index).second.at(bit)).first += count;
        }
        _size += count;
    }

    bool erase(Key key, T count = 1) {
        if (count <= 0 or trie.at(root).first < count) {
            return false;
        }
        auto impl = [&](auto &&self, std::size_t index, std::size_t depth) -> bool {
            if (depth == width) {
                if (trie.at(index).first < count) {
                    return false;
                }
                trie.at(index).first -= count;
                return true;
            }
            Key bit = key >> (width - depth - 1) & 1;
            auto &child = trie.at(index).second.at(bit);
            if (child == invalid or trie.at(child).first < count) {
                return false;
            }
            if (not self(self, child, depth + 1)) {
                return false;
            }
            if (trie.at(index).first -= count; trie.at(child).first == 0) {
                child = invalid;
            }
            return true;
        };
        bool success = impl(impl, root, 0);
        if (success) {
            _size -= count;
        }
        return success;
    }

    T count(Key key) const {
        std::size_t index = root;
        for (std::size_t i = width - 1; i + 1 != 0; --i) {
            Key bit = key >> i & 1;
            auto &child = trie.at(index).second.at(bit);
            if (child == invalid) {
                return 0;
            }
            index = child;
        }
        return trie.at(index).first;
    }

    std::pair<Key, T> limit_xor(Key key, bool max) const {
        Key res = 0;
        std::size_t index = root;
        for (std::size_t i = width - 1; i + 1 != 0; --i) {
            Key bit = (key >> i & 1) ^ max;
            auto &left_child = trie.at(index).second.at(bit);
            bit ^= not (left_child != invalid and trie.at(left_child).first > 0);
            if (auto &right_child = trie.at(index).second.at(bit); right_child != invalid and trie.at(right_child).first > 0) {
                res |= bit << i;
                index = right_child;
            } else {
                return {0, 0};
            }
        }
        return {res ^= key, trie.at(index).first};
    }

    void enumerate(auto &&call) const {
        const auto impl = [&call, this](auto &&self, std::size_t index, Key prefix, std::size_t depth) -> void {
            if (depth == width) {
                std::forward<decltype(call)>(call)(prefix, trie.at(index).first);
                return;
            }
            for (std::size_t bit = 0; bit < 2; ++bit) {
                if (auto &child = trie.at(index).second.at(bit); child != invalid and trie.at(child).first > 0) {
                    self(self, child, prefix | (static_cast<Key>(bit) << (width - depth - 1)), depth + 1);
                }
            }
        };
        impl(impl, root, 0, 0);
    }

    template <typename Ostream>
    friend Ostream &operator<<(Ostream &ostream, const BinaryTrie &self) {
        ostream << '{';
        bool first = true;
        self.enumerate([&](Key key, T count) -> void {
            if (not std::exchange(first, false)) {
                ostream << ", ";
            }
            ostream << key << '*' << count;
        });
        return ostream << '}';
    }

private:
    T _size;
    std::vector<node> trie;
    void allocate_node() {
        trie.emplace_back(T{}, children);
    }
};