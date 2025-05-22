#pragma once
#include <queue>
#include <vector>
#include <tr2/dynamic_bitset>

namespace Dijkstra {
    template <typename Group>
    class Graph {
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
        [[nodiscard]] Info<CountType> solve(std::size_t source, const Group &infinity) const {
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

            return info<CountType>{.infinity = infinity, .source = source, .distances = distances, .number = number, .previous = previous};
        }

    private:
        std::vector<std::vector<std::pair<std::size_t, Group>>> adj;
    };
}