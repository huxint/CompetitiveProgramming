#pragma once
#include <string_view>
#include <ranges>

namespace iostream {
    template <typename T>
    concept range_like = std::ranges::range<T> and not std::is_convertible_v<T, std::string_view>;
    template <typename T>
    concept range_tuple = range_like<T> or std::__is_tuple_like<T>::value and not std::ranges::range<T>;

    template <typename Istream>
    Istream &operator>>(Istream &istream, range_tuple auto &value) {
        if constexpr (range_like<decltype(value)>) {
            for (auto &e : value) {
                istream >> e;
            }
        } else {
            std::apply([&istream](auto &&...args) {
                ((istream >> args), ...);
            }, value);
        }
        return istream;
    }

    template <typename Ostream>
    Ostream &operator<<(Ostream &ostream, const range_tuple auto &value) {
        if constexpr (range_like<decltype(value)>) {
            auto it = value.begin();
            for (ostream << *it++; it != value.end(); ++it) {
                ostream << ' ' << *it;
            }
        } else {
            ostream << '(' << (std::apply([&ostream](auto &&...args) {
                auto size = sizeof...(args);
                ((ostream << args << (--size ? ", " : "")), ...);
            }, value), ')');
        }
        return ostream;
    }
}
using iostream::operator<<, iostream::operator>>;