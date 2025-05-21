#pragma once
#include <vector>
#include <algorithm>
template <typename T>
class Discretizer {
public:
    Discretizer() : _prepared{}, discretizer{} {}

    Discretizer(std::ranges::range auto container) : Discretizer() {
        discretizer.insert(discretizer.end(), container.begin(), container.end());
    }

    template <typename Iterator>
    Discretizer(Iterator begin, Iterator end) : Discretizer(end - begin, [&](auto index) {
        return *(begin + index);
    }) {}

    Discretizer(std::size_t n, auto &&mapping) : Discretizer() {
        for (std::size_t i = 0; i < n; ++i) {
            discretizer.push_back(std::forward<decltype(mapping)>(mapping)(i));
        }
    }

    void push(const T &value) {
        if (not _prepared) {
            discretizer.push_back(value);
            return;
        }
        auto it = std::lower_bound(discretizer.begin(), discretizer.end(), value);
        if (it == discretizer.end()) {
            discretizer.push_back(value);
        } else {
            if (*it != value) {
                discretizer.insert(it, value);
            }
        }
    }

    std::size_t size() const {
        return discretizer.size();
    }

    const T &operator[](std::size_t index) const {
        return discretizer.at(index);
    }

    void prepare() {
        _prepared = true;
        std::sort(discretizer.begin(), discretizer.end());
        discretizer.resize(std::unique(discretizer.begin(), discretizer.end()) - discretizer.begin());
    }

    std::size_t rank(const T &value) const {
        return std::distance(discretizer.begin(), std::lower_bound(discretizer.begin(), discretizer.end(), value));
    }

    template <typename Ostream>
    friend Ostream &operator<<(Ostream &ostream, const Discretizer &self) {
        auto it = self.discretizer.begin();
        for (ostream << *it++; it != self.discretizer.end(); ++it) {
            ostream << ' ' << *it;
        }
        return ostream;
    }

private:
    bool _prepared;
    std::vector<T> discretizer;
};