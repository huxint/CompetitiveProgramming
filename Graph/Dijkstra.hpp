#pragma once
#include <queue>
#include <vector>
#include <tr2/dynamic_bitset>

namespace Dijkstra {
    template <typename Group>
    class Graph {
    public:
        constexpr Graph(std::size_t vertex) : adj(vertex) {}

        constexpr auto add_edge(std::size_t u, std::size_t v, const Group &weight) -> void {
            adj[u].emplace_back(v, weight);
        }

        constexpr auto size() const -> std::size_t {
            return adj.size();
        }

        constexpr auto resize(std::size_t vertex) -> void {
            adj.clear();
            adj.resize(vertex);
        }

        constexpr auto operator[](std::size_t i) -> std::vector<std::pair<std::size_t, Group>> & {
            return adj[i];
        }

        constexpr auto operator[](std::size_t i) const -> const std::vector<std::pair<std::size_t, Group>> & {
            return adj[i];
        }

    private:
        std::vector<std::vector<std::pair<std::size_t, Group>>> adj;
    };

    template <typename Group, typename CountType>
    struct info {
        Group infinity;
        std::size_t source;
        std::vector<Group> distances;
        std::vector<CountType> number;
        std::vector<std::size_t> previous;
        constexpr auto trace(std::size_t end, auto &&call) -> bool {
            if (distances[end] >= infinity) {
                return false;
            }
            auto dfs = [&](auto &&self, std::size_t now, auto &&call) -> void {
                if (now == source) {
                    call(now);
                    return;
                }
                self(self, previous[now], call);
                call(now);
            };
            dfs(dfs, end, call);
            return true;
        }

        constexpr auto path(std::size_t end) -> std::vector<std::size_t> {
            std::vector<std::size_t> res;
            trace(end, [&](std::size_t now) -> void {
                res.push_back(now);
            });
            return res;
        }
    };

    template <typename Group, bool Path = false, bool Count = false, typename CountType = std::size_t>
    constexpr auto solve(const auto &graph, std::size_t source, const Group &infinity) -> decltype(auto) {
        std::vector<Group> distances;
        std::vector<CountType> number;
        std::vector<std::size_t> previous;
        std::tr2::dynamic_bitset<> visited(graph.size());
        distances.resize(graph.size(), infinity);
        distances[source] = Group{};
        if constexpr (Path) {
            previous.resize(graph.size());
        }
        if constexpr (Count) {
            number.resize(graph.size());
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
            for (const auto &[neighbor, weight] : graph[vertex]) {
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
        return info<Group, CountType>{.infinity = infinity, .source = source, .distances = distances, .number = number, .previous = previous};
    }
}