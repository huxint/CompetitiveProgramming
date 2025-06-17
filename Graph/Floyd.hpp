#pragma once
#include <vector>

namespace Floyd {
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

        [[nodiscard]] auto solve(std::size_t source, const Group &infinity) const {
            std::vector distances(adj.size(), std::vector<Group>(adj.size(), infinity));
            for (std::size_t i = 0; i < adj.size(); ++i) {
                distances[i][i] = Group{};
            }

            for (std::size_t i = 0; i < adj.size(); ++i) {
                for (const auto &[j, weight] : adj[i]) {
                    distances[i][j] = weight;
                }
            }

            for (std::size_t k = 0; k < adj.size(); ++k) {
                for (std::size_t i = 0; i < adj.size(); ++i) {
                    for (std::size_t j = 0; j < adj.size(); ++j) {
                        distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
                    }
                }
            }

            return distances;
        }
    private:
        std::vector<std::vector<std::pair<std::size_t, Group>>> adj;
    };
}
