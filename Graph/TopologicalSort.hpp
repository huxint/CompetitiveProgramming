/**
 * @brief 拓扑排序算法（Topological Sort）
 * @details 对有向无环图（DAG）进行拓扑排序，返回一个线性序列
 *          使用Kahn算法实现，基于入度的BFS方法
 * @complexity O(V + E), 其中V为顶点数，E为边数
 */
#pragma once
#include <queue>
#include <vector>
namespace TopologicalSort {
    class Graph {
    public:
        Graph(std::size_t vertex) : adj(vertex), _degree(vertex) {}

        void add_edge(std::size_t u, std::size_t v) {
            adj[u].push_back(v);
            ++_degree[v];
        }

        std::size_t size() const {
            return adj.size();
        }

        void resize(std::size_t vertex) {
            adj.clear();
            _degree.clear();
            adj.resize(vertex);
            _degree.resize(vertex);
        }

        const std::vector<std::size_t> &operator[](std::size_t index) const {
            return adj[index];
        }

        std::size_t degree(std::size_t index) const {
            return _degree[index];
        }

        [[nodiscard]] std::vector<std::size_t> solve() const {
            std::vector<std::size_t> res;
            std::vector<std::size_t> deg(_degree);

            std::queue<std::size_t> Q;
            for (std::size_t i = 0; i < adj.size(); ++i) {
                if (deg[i] == 0) {
                    Q.push(i);
                }
            }

            while (not Q.empty()) {
                auto u = Q.front();
                Q.pop();
                res.push_back(u);

                for (const auto &v : adj[u]) {
                    if (--deg[v] == 0) {
                        Q.push(v);
                    }
                }
            }

            if (res.size() != adj.size()) {
                res.clear();
            }
            return res;
        }

    private:
        std::vector<std::size_t> _degree;
        std::vector<std::vector<std::size_t>> adj;
    };
}