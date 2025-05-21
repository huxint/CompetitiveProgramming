#pragma once
#include <vector>

namespace TreeDiameter {
    template <typename T>
    struct Info {
        std::size_t source;
        std::size_t target;
        std::vector<T> distances;
        std::vector<std::size_t> previous;

        T distance() const {
            return distances.at(target);
        }

        void trace(auto &&call) const {
            std::size_t leaf = target;
            do {
                std::forward<decltype(call)>(call)(leaf);
            } while (~(leaf = previous[leaf]));
        }

        std::vector<std::size_t> path() const {
            std::vector<std::size_t> res;
            res.reserve(distance() + 1);
            trace([&](auto leaf) {
                res.push_back(leaf);
            });
            return res;
        }
    };

    template <typename T>
    Info<T> solve(const auto &tree) {
        std::size_t source;
        std::size_t target;
        std::vector<T> distances;
        std::vector<std::size_t> previous;
        auto bfs = [&](std::size_t start) -> Info<T> {
            std::vector<T> distances(n, std::numeric_limits<T>::max());
            std::vector<std::size_t> previous(n, -1);
            std::queue<std::size_t> q;

            distances[start] = 0;
            q.push(start);

            while (not q.empty()) {
                std::size_t u = q.front();
                q.pop();

                for (const auto &[v, weight] : tree[u]) {
                    if (distances[v] > distances[u] + weight) {
                        distances[v] = distances[u] + weight;
                        previous[v] = u;
                        q.push(v);
                    }
                }
            }

            // Find the node with maximum distance
            std::size_t farthest = start;
            for (std::size_t i = 0; i < n; ++i) {
                if (distances[i] != std::numeric_limits<T>::max() &&
                    (distances[i] > distances[farthest] || farthest == start)) {
                    farthest = i;
                }
            }

            return {start, farthest, distances, previous};
        };
    }
}