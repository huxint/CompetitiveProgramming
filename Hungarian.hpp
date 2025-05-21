#pragma once
#include <vector>
#include <tr2/dynamic_bitset>

namespace Hungarian {
    constexpr auto none = std::size_t(-1);
    class Graph {
    public:
        Graph(std::size_t vertex) : adj(vertex) {}

        void add_edge(std::size_t left, std::size_t right) {
            adj[left].push_back(right);
        }

        std::size_t size() const {
            return adj.size();
        }

        void resize(std::size_t vertex) {
            adj.clear();
            adj.resize(vertex);
        }

        std::vector<std::size_t> &operator[](std::size_t index) {
            return adj[index];
        }

        const std::vector<std::size_t> &operator[](std::size_t index) const {
            return adj[index];
        }

    private:
        std::vector<std::vector<std::size_t>> adj;
    };

    namespace MaximumMatch {
        auto solve(const auto &graph) {
            std::size_t max = 0;
            std::tr2::dynamic_bitset<> visited(graph.size());
            std::vector<std::size_t> left_info(graph.size(), none);
            std::vector<std::size_t> right_info(graph.size(), none);
            auto match = [&](auto &&self, std::size_t left) -> bool {
                for (const auto &right : graph[left]) {
                    if (not visited.test(right)) {
                        visited.set(right);
                        if (auto &_ = right_info[right]; _ == none or self(self, _)) {
                            _ = left;
                            return true;
                        }
                    }
                }
                return false;
            };
            for (std::size_t left = 0; left < graph.size(); ++left) {
                visited.reset();
                max += match(match, left);
            }
            for (std::size_t left = 0; left < graph.size(); ++left) {
                if (right_info[left] == none) {
                    continue;
                }
                left_info[right_info[left]] = left;
            }
            struct info {
                std::size_t max;
                std::vector<std::size_t> left_info;
                std::vector<std::size_t> right_info;
            };
            return info{.max = max, .left_info = left_info, .right_info = right_info};
        }
    }
}