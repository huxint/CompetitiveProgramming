#pragma once
#include <vector>
#include <tr2/dynamic_bitset>

/*
二分图最大匹配
*/
namespace Hungarian {
    constexpr std::size_t none = -1;
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

        const std::vector<std::size_t> &operator[](std::size_t index) const {
            return adj[index];
        }

        [[nodiscard]] auto solve() const {
            std::size_t max = 0;
            std::tr2::dynamic_bitset<> visited(adj.size());
            std::vector<std::size_t> left_info(adj.size(), none);
            std::vector<std::size_t> right_info(adj.size(), none);

            auto match = [&](auto &&self, std::size_t left) -> bool {
                for (const auto &right : adj[left]) {
                    if (not visited.test(right)) {
                        visited.set(right);
                        if (auto &matching = right_info[right]; matching == none or self(self, matching)) {
                            matching = left;
                            return true;
                        }
                    }
                }
                return false;
            };

            for (std::size_t left = 0; left < adj.size(); ++left) {
                visited.reset();
                max += match(match, left);
            }

            for (std::size_t left = 0; left < adj.size(); ++left) {
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

    private:
        std::vector<std::vector<std::size_t>> adj;
    };
}