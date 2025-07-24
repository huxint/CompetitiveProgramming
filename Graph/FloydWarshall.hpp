/**
 * @brief Floyd-Warshall传递闭包算法
 * @details 使用bitset优化的传递闭包实现，用于判断图中任意两点间的可达性
 *          适用于稠密图的传递闭包计算
 * @complexity O(V^3 / w), 其中V为顶点数，w为机器字长
 */
#pragma once
#include <tr2/dynamic_bitset>
#include <vector>
namespace FloydWarshall {
    /* Floyd Warshall Algorithm 传递闭包 */
    class Graph {
    public:
        Graph(std::size_t vertex) : adj(vertex, std::tr2::dynamic_bitset<>(vertex)) {}

        void add_edge(std::size_t u, std::size_t v) {
            adj[u].set(v);
        }

        void resize(std::size_t vertex) {
            adj.clear();
            adj.resize(vertex, std::tr2::dynamic_bitset<>(vertex));
        }

        std::tr2::dynamic_bitset<> &operator[](std::size_t i) {
            return adj[i];
        }

        const std::tr2::dynamic_bitset<> &operator[](std::size_t i) const {
            return adj[i];
        }

        [[nodiscard]] std::vector<std::tr2::dynamic_bitset<>> solve() const {
            auto res = adj;
            for (std::size_t k = 0; k != adj.size(); ++k) {
                for (std::size_t i = 0; i != adj.size(); ++i) {
                    if (res[i][k]) {
                        res[i] |= res[k];
                    }
                }
            }
            return res;
        }

    private:
        std::vector<std::tr2::dynamic_bitset<>> adj;
    };
}