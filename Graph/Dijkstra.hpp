/**
 * @brief Dijkstra最短路算法
 * @details 支持单源最短路径查询，包括路径记录、路径计数、最短路计数等功能
 *          可通过重载比较运算符处理复杂的权值类型
 *          使用示例：
 *          - Dijkstra::Graph<int> adj(n); adj.add_edge(u, v, w); adj.solve(start, std::numeric_limits<int>::max() / 2);
 *          - adj.solve<true>(start, std::numeric_limits<int>::max() / 2); // 记录路径
 *          - adj.solve<false, true>(start, std::numeric_limits<int>::max() / 2); // 计数最短路
 *          - adj.solve<true, true, modint>(start, std::numeric_limits<int>::max() / 2); // 路径记录+计数（使用取模类）
 * @complexity O((V + E) log V), 其中V为顶点数，E为边数
 */
#pragma once
#include <queue>
#include <vector>
#include <tr2/dynamic_bitset>
namespace Dijkstra {
    template <typename Group>
    class Graph {
    private:
        template <typename CountType>
        struct info {
            Group infinity;
            std::size_t source;
            std::vector<Group> distances;
            std::vector<CountType> number;
            std::vector<std::size_t> previous;
            bool trace(std::size_t end, auto &&call) {
                if (distances[end] >= infinity) {
                    return false;
                }
                auto dfs = [&](auto &&self, std::size_t now, auto &&call) -> void {
                    if (now == source) {
                        std::forward<decltype(call)>(call)(now);
                        return;
                    }
                    self(self, previous[now], std::forward<decltype(call)>(call));
                    std::forward<decltype(call)>(call)(now);
                };
                dfs(dfs, end, std::forward<decltype(call)>(call));
                return true;
            }

            std::vector<std::size_t> path(std::size_t end) {
                std::vector<std::size_t> res;
                trace(end, [&](std::size_t now) -> void {
                    res.push_back(now);
                });
                return res;
            }
        };
    public:
        Graph(std::size_t vertex) : adj(vertex) {}

        void add_edge(std::size_t u, std::size_t v, const Group &weight) {
            adj[u].emplace_back(v, weight);
        }

        std::size_t size() const {
            return adj.size();
        }

        void resize(std::size_t vertex) {
            adj.clear();
            adj.resize(vertex);
        }

        const std::vector<std::pair<std::size_t, Group>> &operator[](std::size_t i) const {
            return adj[i];
        }

        template <bool Path = false, bool Count = false, typename CountType = std::size_t>
        [[nodiscard]] auto solve(std::size_t source, const Group &infinity) const {
            std::vector<Group> distances;
            std::vector<CountType> number;
            std::vector<std::size_t> previous;
            std::tr2::dynamic_bitset<> visited(adj.size());
            distances.resize(adj.size(), infinity);
            distances[source] = Group{};

            if constexpr (Path) {
                previous.resize(adj.size());
            }

            if constexpr (Count) {
                number.resize(adj.size());
                number[source] = CountType{1};
            }

            std::priority_queue<std::pair<Group, std::size_t>, std::vector<std::pair<Group, std::size_t>>, std::greater<>> heap;
            heap.emplace(Group{}, source);

            while (not heap.empty()) {
                auto [_, vertex] = heap.top();
                heap.pop();
                if (visited.test(vertex)) {
                    continue;
                }
                visited.set(vertex);
                for (const auto &[neighbor, weight] : adj[vertex]) {
                    auto &neighbor_dist = distances[neighbor];
                    auto new_dist = distances[vertex] + weight;
                    if (neighbor_dist > new_dist) {
                        neighbor_dist = new_dist;
                        if constexpr (Count) {
                            number[neighbor] = number[vertex];
                        }
                        heap.emplace(neighbor_dist, neighbor);
                        if constexpr (Path) {
                            previous[neighbor] = vertex;
                        }
                    } else if (neighbor_dist == new_dist) {
                        if constexpr (Count) {
                            number[neighbor] += number[vertex];
                        }
                    }
                }
            }
            return info<CountType>{.infinity = infinity, .source = source, .distances = distances, .number = number, .previous = previous};
        }

    private:
        std::vector<std::vector<std::pair<std::size_t, Group>>> adj;
    };
}